#include "SurfAvgRadiationEmissionBC.h"

registerMooseObject("PhoenixApp", SurfAvgRadiationEmissionBC);

defineLegacyParams(SurfAvgRadiationEmissionBC);

InputParameters
SurfAvgRadiationEmissionBC::validParams()
{
  InputParameters params = IntegratedBC::validParams();
  params += RadiationBCInterface::validParams();
  params += AverageRadiationBCInterface::validParams();

  params.addClassDescription("BC class for average radiation emission.");
  return params;
}

SurfAvgRadiationEmissionBC::SurfAvgRadiationEmissionBC(const InputParameters & parameters)
  : IntegratedBC(parameters),
    RadiationBCInterface(this),
    AverageRadiationBCInterface(this)
{
}

void
SurfAvgRadiationEmissionBC::computeResidual()
{
  _T4_avg = _avg_rad_flux_helper.getSurfAvgRes(_current_boundary_id);
  IntegratedBC::computeResidual();
}

Real
SurfAvgRadiationEmissionBC::computeQpResidual()
{
  return _test[_i][_qp] * _sigma * _T4_avg;
}

Real
SurfAvgRadiationEmissionBC::computeQpJacobian()
{
  SurfaceID surf_id = {*_current_elem, _current_side};
  Real T4_avg_dT = _avg_rad_flux_helper.getSurfAvgJac(surf_id, _current_boundary_id, _j);
  return _test[_i][_qp] * _sigma * T4_avg_dT;
}
