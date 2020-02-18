#include "AverageRadiationEmissionBC.h"

registerMooseObject("PhoenixApp", AverageRadiationEmissionBC);

defineLegacyParams(AverageRadiationEmissionBC);

InputParameters
AverageRadiationEmissionBC::validParams()
{
  InputParameters params = AverageRadiationBC::validParams();
  params.addClassDescription("BC class for average radiation emission.");
  return params;
}

AverageRadiationEmissionBC::AverageRadiationEmissionBC(const InputParameters & parameters)
  : AverageRadiationBC(parameters)
{
}

Real
AverageRadiationEmissionBC::computeQpResidual()
{
  Real T4_avg = _avg_rad_flux_helper.getResidual(*_current_elem, _current_side);
  return _test[_i][_qp] * _sigma * T4_avg;
}

Real
AverageRadiationEmissionBC::computeQpJacobian()
{
  Real T4_avg_dT = _avg_rad_flux_helper.getJacobian(*_current_elem, _current_side, _j);
  return _test[_i][_qp] * _sigma * T4_avg_dT;
}
