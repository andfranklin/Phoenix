#include "LocalRadiationEmissionBC.h"

registerMooseObject("PhoenixApp", LocalRadiationEmissionBC);

defineLegacyParams(LocalRadiationEmissionBC);

InputParameters
LocalRadiationEmissionBC::validParams()
{
  InputParameters params = RadiationBC::validParams();
  params.addClassDescription("BC class for localized radiation emission.");
  return params;
}

LocalRadiationEmissionBC::LocalRadiationEmissionBC(const InputParameters & parameters)
  : RadiationBC(parameters)
{
}

Real
LocalRadiationEmissionBC::computeQpResidual()
{
  Real T4 = _u[_qp] * _u[_qp];
  T4 = T4 * T4;
  return _test[_i][_qp] * _sigma * T4;
}

Real
LocalRadiationEmissionBC::computeQpJacobian()
{
  Real T3 = _u[_qp] * _u[_qp] * _u[_qp];
  return _test[_i][_qp] * _sigma * 4.0 * T3 * _phi[_j][_qp];
}
