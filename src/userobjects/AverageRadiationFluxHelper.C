#include "AverageRadiationFluxHelper.h"

registerMooseObject("PhoenixApp", AverageRadiationFluxHelper);

template <>
InputParameters
validParams<AverageRadiationFluxHelper>()
{
  InputParameters params = validParams<ShapeSideUserObject>();

  ExecFlagEnum exec_enum = MooseUtils::getDefaultExecFlagEnum();
  exec_enum  = EXEC_LINEAR;
  exec_enum += EXEC_NONLINEAR;
  params.set<ExecFlagEnum>("execute_on", true) = exec_enum;

  params.addRequiredCoupledVar("variable", "The name of the temperature variable.");

  return params;
}

AverageRadiationFluxHelper::AverageRadiationFluxHelper(const InputParameters & parameters)
  : ShapeSideUserObject(parameters),
  MooseVariableInterface<Real>(this,
                               false,
                               "variable",
                               Moose::VarKindType::VAR_ANY,
                               Moose::VarFieldType::VAR_FIELD_STANDARD),
  _temp(coupledValue("variable")),
  _grad_temp(coupledGradient("variable"))
{
  addMooseVariableDependency(mooseVariable());
}

void
AverageRadiationFluxHelper::initialize()
{
  for (const auto & boundary_id : boundaryIDs())
  {
    _boundary_surface_areas[boundary_id] = 0.0;
    _boundary_residuals[boundary_id] = 0.0;
  }
}

void
AverageRadiationFluxHelper::execute()
{
  Real elem_area = _current_side_elem->volume();
  _boundary_surface_areas[_current_boundary_id] += elem_area;

  if (computeJacobianFlag() && _fe_problem.currentlyComputingJacobian())
    computeJacobian();
  else
    computeResidual();
}

Real
AverageRadiationFluxHelper::computeQpJacobian()
{
  Real temp3 = _temp[_qp] * _temp[_qp] * _temp[_qp];
  return 4.0 * temp3 * _phi[_j][_qp];
}

void
AverageRadiationFluxHelper::computeJacobian()
{
  DenseVector<Real> jac;
  jac.resize(_phi.size());
  for (_j = 0; _j < _phi.size(); _j++)
    for (_qp = 0; _qp < _qrule->n_points(); _qp++)
      jac(_j) += _JxW[_qp] * _coord[_qp] * computeQpJacobian();

  SurfaceID surf_id = {*_current_elem, _current_side};
  _boundary_jacobians[surf_id] = jac;

  Real elem_area = _current_side_elem->volume();
  jac.scale(1.0 / elem_area);
  _element_surface_to_jac[surf_id] = jac;
}

Real
AverageRadiationFluxHelper::computeQpResidual()
{
  Real temp2 = _temp[_qp] * _temp[_qp];
  return temp2 * temp2;
}

void
AverageRadiationFluxHelper::computeResidual()
{
  Real res = 0.0;
  for (_qp = 0; _qp < _qrule->n_points(); _qp++)
    res += _JxW[_qp] * _coord[_qp] * computeQpResidual();

  _boundary_residuals[_current_boundary_id] += res;
  Real elem_area = _current_side_elem->volume();
  _element_surface_to_res[{*_current_elem, _current_side}] = res / elem_area;
}

Real
AverageRadiationFluxHelper::getElemAvgRes(const SurfaceID & surf_id) const
{
  return _element_surface_to_res.at(surf_id);
}

Real
AverageRadiationFluxHelper::getElemAvgJac(const SurfaceID & surf_id, unsigned int j) const
{
  const auto & jacs = _element_surface_to_jac.at(surf_id);

  // kludge
  if (j >= jacs.size())
    return 0.0;
  else
    return jacs(j);
}

Real
AverageRadiationFluxHelper::getSurfAvgRes(const BoundaryID & boundary_id) const
{
  Real res = _boundary_residuals.at(boundary_id);
  Real sas = _boundary_surface_areas.at(boundary_id);
  return res / sas;
}

Real
AverageRadiationFluxHelper::getSurfAvgJac(const SurfaceID & surf_id, const BoundaryID & boundary_id, unsigned int j) const
{
  const auto & jacs = _boundary_jacobians.at(surf_id);

  // kludge
  Real jacobian = 0.0;
  if (j < jacs.size())
    jacobian = jacs(j);

  return jacobian / _boundary_surface_areas.at(boundary_id);
}
