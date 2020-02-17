#include "AverageRadiationEmissionBC.h"

registerMooseObject("PhoenixApp", AverageRadiationEmissionBC);

defineLegacyParams(AverageRadiationEmissionBC);

InputParameters
AverageRadiationEmissionBC::validParams()
{
  InputParameters params = RadiationBC::validParams();
  params.addClassDescription("BC class for average radiation emission.");
  return params;
}

AverageRadiationEmissionBC::AverageRadiationEmissionBC(const InputParameters & parameters)
  : RadiationBC(parameters)
{
}

void
AverageRadiationEmissionBC::computeResidual()
{
  computeResidualSetup();
  IntegratedBC::computeResidual();
}

void
AverageRadiationEmissionBC::computeJacobian()
{
  computeJacobianSetup();
  IntegratedBC::computeJacobian();
}

void
AverageRadiationEmissionBC::computeResidualSetup()
{
  Real tmp;
  _T4_avg = 0.0;
  for (_qp = 0; _qp < _qrule->n_points(); _qp++)
  {
    tmp = _u[_qp] * _u[_qp];
    tmp = tmp * tmp;
    _T4_avg += _JxW[_qp] * _coord[_qp] * tmp;
  }
  Real elem_area = _current_side_elem->volume();
  _T4_avg = _T4_avg / elem_area;
}

void
AverageRadiationEmissionBC::computeJacobianSetup()
{
  Real elem_area = _current_side_elem->volume();
  Real tmp;

  _T4_avg_dT.resize(_phi.size());
  for (_j = 0; _j < _phi.size(); _j++)
  {
    for (_qp = 0; _qp < _qrule->n_points(); _qp++)
    {
      tmp = _u[_qp] * _u[_qp] * _u[_qp];
      _T4_avg_dT(_j) += _JxW[_qp] * _coord[_qp] * 4.0 * tmp;
    }
    _T4_avg_dT(_j) = _T4_avg_dT(_j) / elem_area;
  }
}

Real
AverageRadiationEmissionBC::computeQpResidual()
{
  return _test[_i][_qp] * _sigma * _T4_avg;
}

Real
AverageRadiationEmissionBC::computeQpJacobian()
{
  return _test[_i][_qp] * _sigma * _T4_avg_dT(_j);
}
