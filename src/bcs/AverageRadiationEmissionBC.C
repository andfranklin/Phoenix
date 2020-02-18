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
  SurfaceID surf_id = {*_current_elem, _current_side};
  Real T4_avg = _avg_rad_flux_helper.getResidual(surf_id);
  return _test[_i][_qp] * _sigma * T4_avg;
}

Real
AverageRadiationEmissionBC::computeQpJacobian()
{
  SurfaceID surf_id = {*_current_elem, _current_side};
  Real T4_avg_dT = _avg_rad_flux_helper.getJacobian(surf_id, _j);
  return _test[_i][_qp] * _sigma * T4_avg_dT;
}
