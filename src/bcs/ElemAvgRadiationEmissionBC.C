#include "ElemAvgRadiationEmissionBC.h"

registerMooseObject("PhoenixApp", ElemAvgRadiationEmissionBC);

defineLegacyParams(ElemAvgRadiationEmissionBC);

InputParameters
ElemAvgRadiationEmissionBC::validParams()
{
  InputParameters params = IntegratedBC::validParams();
  params += RadiationBCInterface::validParams();
  params += AverageRadiationBCInterface::validParams();

  params.addClassDescription("BC class for average radiation emission.");
  return params;
}

ElemAvgRadiationEmissionBC::ElemAvgRadiationEmissionBC(const InputParameters & parameters)
  : IntegratedBC(parameters),
    RadiationBCInterface(this),
    AverageRadiationBCInterface(this)
{
}

Real
ElemAvgRadiationEmissionBC::computeQpResidual()
{
  SurfaceID surf_id = {*_current_elem, _current_side};
  Real T4_avg = _avg_rad_flux_helper.getResidual(surf_id);
  return _test[_i][_qp] * _sigma * T4_avg;
}

Real
ElemAvgRadiationEmissionBC::computeQpJacobian()
{
  SurfaceID surf_id = {*_current_elem, _current_side};
  Real T4_avg_dT = _avg_rad_flux_helper.getJacobian(surf_id, _j);
  return _test[_i][_qp] * _sigma * T4_avg_dT;
}
