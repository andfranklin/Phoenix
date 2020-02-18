#ifndef RADIATIONHEATTRANSFERSETUP_H
#define RADIATIONHEATTRANSFERSETUP_H

#include "SideUserObject.h"
// #include "libmesh/quadrature.h"
#include "SurfaceConnectorBase.h"
#include "GeometryUtils.h"

#include "NoOcclusionSurfaceConnector.h"
#include "BruteForceWithBBCSurfaceConnector.h"
#include "BruteForceWithoutBBCSurfaceConnector.h"

class ViewFactorCalculator;

template <>
InputParameters validParams<ViewFactorCalculator>();

class ViewFactorCalculator : public SideUserObject
{
public:
  ViewFactorCalculator(const InputParameters & parameters);
  virtual ~ViewFactorCalculator();

  virtual void initialize() override;
  virtual void execute() override;
  virtual void threadJoin(const UserObject & y) override;
  virtual void finalize() override;

  inline const Node * getVertex(unsigned int index) { return _current_side_elem->node_ptr(index); }

protected:
  MooseEnum _quad_type_enum;
  QuadratureType _specified_quad_type;

  MooseEnum _quad_order_enum;
  Order _specified_quad_order;

  MooseEnum _occlusion_detection;

  MooseEnum _base_repr;
  MooseEnum _quad_repr;
  MooseEnum _collision_repr;

  const std::set<BoundaryID> & _boundary_ids;
  std::map<BoundaryID, std::vector<unsigned int>> _boundary_index_sets;

  Geom::SurfaceConnectorBase * _surface_connector;
  unsigned int _current_surface_index;

  unsigned int _precision;

protected:
  QuadratureType getQuadratureType();
  Order getQuadratureOrder();
  unsigned int getQuadratureDimension();

  void setupSurfaceConnector();

  template<typename BaseKernel>
  void applyBase();

  template<typename BaseKernel, typename QuadKernel>
  void applyQuad();

  template<typename BaseKernel, typename QuadKernel, typename CollisionKernel>
  void buildSurfaceConnector();
};

template<typename BaseKernel>
inline void
ViewFactorCalculator::applyBase()
{
  if (_quad_repr == "EXACT")
    applyQuad<BaseKernel, Geom::Exact>();
  else
    applyQuad<BaseKernel, Geom::Apprx>();
}

template<typename BaseKernel, typename QuadKernel>
inline void
ViewFactorCalculator::applyQuad()
{
  if (_collision_repr == "EXACT")
    buildSurfaceConnector<BaseKernel, QuadKernel, Geom::Exact>();
  else
    buildSurfaceConnector<BaseKernel, QuadKernel, Geom::Apprx>();
}

template<typename BaseKernel, typename QuadKernel, typename CollisionKernel>
inline void
ViewFactorCalculator::buildSurfaceConnector()
{
  if (_occlusion_detection == "NONE")
  {
    using surface_connector_t = Geom::NoOcclusionSurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>;
    _surface_connector = new surface_connector_t();
  }
  else if (_occlusion_detection == "BRUTE_FORCE_WITH_BBOX")
  {
    using surface_connector_t = Geom::BruteForceWithBBCSurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>;
    _surface_connector = new surface_connector_t();
  }
  else
  {
    using surface_connector_t = Geom::BruteForceWithoutBBCSurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>;
    _surface_connector = new surface_connector_t();
  }
}

#endif /* RADIATIONHEATTRANSFERSETUP_H */