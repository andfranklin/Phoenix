#include "RadiationHeatTransferSetup.h"
#include "Conversion.h"
#include "Assembly.h"

registerMooseObject("PhoenixApp", RadiationHeatTransferSetup);

template <>
InputParameters
validParams<RadiationHeatTransferSetup>()
{
  InputParameters params = validParams<SideUserObject>();

  MooseEnum quad_types("CLOUGH CONICAL GAUSS GRID MONOMIAL SIMPSON TRAP GAUSS_LOBATTO DEFAULT",
                  "DEFAULT");
  MooseEnum quad_order("CONSTANT FIRST SECOND THIRD FOURTH FIFTH SIXTH SEVENTH EIGHTH NINTH TENTH "
                  "ELEVENTH TWELFTH THIRTEENTH FOURTEENTH FIFTEENTH SIXTEENTH SEVENTEENTH "
                  "EIGHTTEENTH NINTEENTH TWENTIETH DEFAULT",
                  "DEFAULT");

  params.addParam<MooseEnum>(
      "quadrature_type",
      quad_types,
      "Type of the quadrature rule. Defaults to the global side quadrature type.");
  params.addParam<MooseEnum>(
      "quadrature_order",
      quad_order,
      "Order of the quadrature. Defaults to the global side quadrature order.");

  MooseEnum occlusion_detection("NONE BRUTE_FORCE_WITH_BBOX BRUTE_FORCE_WITHOUT_BBOX",
                  "BRUTE_FORCE_WITHOUT_BBOX");

  params.addParam<MooseEnum>("occlusion_detection", occlusion_detection,
    "Method by which the occlusion of rays by surfaces is detected.");

  MooseEnum geom_representation("EXACT APPROXIMATE", "APPROXIMATE");

  params.addParam<MooseEnum>("base_representation", geom_representation, "description");
  params.addParam<MooseEnum>("quadrature_representation", geom_representation, "the type of mathematical kernel used when conducting quadratures");
  params.addParam<MooseEnum>("collision_representation", geom_representation, "the type of mathematical kernel used to determine collisions");

  params.addParam<unsigned int>("precision", 5, "Precision to which the output should be printed");

  ExecFlagEnum exec_enum = MooseUtils::getDefaultExecFlagEnum();
  exec_enum  = EXEC_INITIAL;
  params.set<ExecFlagEnum>("execute_on", true) = exec_enum;

  return params;
}

RadiationHeatTransferSetup::RadiationHeatTransferSetup(const InputParameters & parameters)
  : SideUserObject(parameters),

    _quad_type_enum(getParam<MooseEnum>("quadrature_type")),
    _specified_quad_type(Moose::stringToEnum<QuadratureType>(_quad_type_enum)),

    _quad_order_enum(getParam<MooseEnum>("quadrature_order")),
    _specified_quad_order(Moose::stringToEnum<Order>(_quad_order_enum)),

    _occlusion_detection(getParam<MooseEnum>("occlusion_detection")),

    _base_repr(getParam<MooseEnum>("base_representation")),
    _quad_repr(getParam<MooseEnum>("quadrature_representation")),
    _collision_repr(getParam<MooseEnum>("collision_representation")),

    _boundary_ids(boundaryIDs()),
    _boundary_index_sets(),

    _surface_connector(nullptr),
    _current_surface_index(0),

    _precision(getParam<unsigned int>("precision"))
{
  setupSurfaceConnector();

  for (auto & boundary_id : _boundary_ids)
    _boundary_index_sets[boundary_id] = {};
}

RadiationHeatTransferSetup::~RadiationHeatTransferSetup()
{
  delete _surface_connector;
}

inline void
RadiationHeatTransferSetup::setupSurfaceConnector()
{
  if (_base_repr == "EXACT")
    applyBase<Geom::Exact>();
  else
    applyBase<Geom::Apprx>();
}

inline QuadratureType
RadiationHeatTransferSetup::getQuadratureType()
{
  return _quad_type_enum == "DEFAULT" ? _qrule->type() : _specified_quad_type;
}

inline Order
RadiationHeatTransferSetup::getQuadratureOrder()
{
  return _quad_order_enum == "DEFAULT" ? _qrule->get_order() : _specified_quad_order;
}

inline unsigned int
RadiationHeatTransferSetup::getQuadratureDimension()
{
  return _qrule->get_dim();
}

void
RadiationHeatTransferSetup::initialize()
{
  _current_surface_index = 0;
}

void
RadiationHeatTransferSetup::execute()
{
  if (_surface_connector->quadraturesNotInitialized())
  {
    _surface_connector->setQuadratureType(getQuadratureType());
    _surface_connector->setQuadratureOrder(getQuadratureOrder());
    _surface_connector->setQuadratureDimension(getQuadratureDimension());
    _surface_connector->initializeQuadratures();
  }

  BoundaryID current_boundary_id = _mesh.getBoundaryIDs(_current_elem, _current_side)[0];
  _boundary_index_sets[current_boundary_id].push_back(_current_surface_index);

  switch (_current_side_elem->n_vertices())
  {
    case 3:
    {
      _surface_connector->buildSurface(getVertex(0), getVertex(1), getVertex(2));
      break;
    }
    case 4:
    {
      _surface_connector->buildSurface(getVertex(0), getVertex(1), getVertex(2), getVertex(3));
      break;
    }
    default:
    {
      mooseError("Undefined Surface Type");
      break;
    }
  }

  ++_current_surface_index;
}

void
RadiationHeatTransferSetup::threadJoin(const UserObject & /*y*/)
{
}

void
RadiationHeatTransferSetup::finalize()
{
  bool infeasible_vf_warning = false;

  for (auto from_bnd_id : _boundary_ids)
  {
    const auto & from_bnd_name = _mesh.getBoundaryName(from_bnd_id);
    const auto & from_bnd_inxs = _boundary_index_sets[from_bnd_id];

    std::cout << from_bnd_name << " (" << from_bnd_id << ") : " << from_bnd_inxs.size() << std::endl;

    for (auto to_bnd_id : _boundary_ids)
    {
      const auto & to_bnd_name = _mesh.getBoundaryName(to_bnd_id);
      const auto & to_bnd_inxs = _boundary_index_sets[to_bnd_id];
      std::cout << "  " << from_bnd_name << " -> " << to_bnd_name << std::endl;

      Real total_area = 0.0;
      Real view_factor = 0.0;
      for (auto from_index : from_bnd_inxs) {

        Real outer_area = _surface_connector->getArea(from_index);
        total_area += outer_area;

        for (auto to_index : to_bnd_inxs) {
          Real intermediate = _surface_connector->getViewFactor(from_index, to_index);
          view_factor += outer_area * intermediate;
        }
      }

      view_factor = view_factor / total_area;

      if (view_factor > 1.0) infeasible_vf_warning = true;
      std::cout << "    view factor : " << std::setprecision(_precision) << view_factor << std::endl << std::endl;
      // std::cout << "  total SA    : " << total_area << std::endl << std::endl;
    }
  }

  if (infeasible_vf_warning)
    mooseWarning("One or more surface view factors are greater than one. Consider refining the mesh, increasing the quadrature order or both.");
}
