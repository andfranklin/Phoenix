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
AverageRadiationFluxHelper::execute()
{
  if (computeJacobianFlag() && _fe_problem.currentlyComputingJacobian())
    computeJacobian();
  else
    computeResidual();
}

Real
AverageRadiationFluxHelper::computeQpJacobian()
{
  Real temp3 = _temp[_qp] * _temp[_qp] * _temp[_qp];
  Real elem_area = _current_side_elem->volume();
  return (4.0 * temp3 * _phi[_j][_qp]) / elem_area;
}

void
AverageRadiationFluxHelper::computeJacobian()
{
  DenseVector<Real> jac;
  jac.resize(_phi.size());
  for (_j = 0; _j < _phi.size(); _j++)
    for (_qp = 0; _qp < _qrule->n_points(); _qp++)
      jac(_j) += _JxW[_qp] * _coord[_qp] * computeQpJacobian();

  SideElemKey key = {_current_elem->unique_id(), _current_side};
  _element_surface_to_jac[key] = jac;
}

Real
AverageRadiationFluxHelper::computeQpResidual()
{
  Real temp2 = _temp[_qp] * _temp[_qp];
  Real elem_area = _current_side_elem->volume();
  return (temp2 * temp2) / elem_area;
}

void
AverageRadiationFluxHelper::computeResidual()
{
  Real res = 0.0;
  for (_qp = 0; _qp < _qrule->n_points(); _qp++)
    res += _JxW[_qp] * _coord[_qp] * computeQpResidual();

  SideElemKey key = {_current_elem->unique_id(), _current_side};
  _element_surface_to_res[key] = res;
}

Real
AverageRadiationFluxHelper::getResidual(const Elem & elem, unsigned int side) const
{
  return _element_surface_to_res.at({elem.unique_id(), side});
}

Real
AverageRadiationFluxHelper::getJacobian(const Elem & elem, unsigned int side, unsigned int j) const
{
  const auto & jacs = _element_surface_to_jac.at({elem.unique_id(), side});

  // kludge
  if (j >= jacs.size())
    return 0.0;
  else
    return jacs(j);
}
