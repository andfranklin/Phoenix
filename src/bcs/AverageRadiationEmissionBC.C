#include "AverageRadiationEmissionBC.h"

registerMooseObject("PhoenixApp", AverageRadiationEmissionBC);

defineLegacyParams(AverageRadiationEmissionBC);

InputParameters
AverageRadiationEmissionBC::validParams()
{
  InputParameters params = RadiationBC::validParams();
  params.addRequiredParam<UserObjectName>("avg_rad_flux_helper",
                                          "Computes the average radiation flux local to each element's face.");
  params.addClassDescription("BC class for average radiation emission.");
  return params;
}

AverageRadiationEmissionBC::AverageRadiationEmissionBC(const InputParameters & parameters)
  : RadiationBC(parameters),
  _avg_rad_flux_helper(getUserObject<AverageRadiationFluxHelper>("avg_rad_flux_helper"))
{
  // TODO: Check that the _avg_rad_flux_helper computes on the side that the
  // BC is specified for.
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
