#include "AverageIrradiationBC.h"

registerMooseObject("PhoenixApp", AverageIrradiationBC);

defineLegacyParams(AverageIrradiationBC);

InputParameters
AverageIrradiationBC::validParams()
{
  InputParameters params = AverageRadiationBC::validParams();

  params.addRequiredParam<UserObjectName>("view_factor_calculator",
                                          "Interface to the view factors.");

  params.addClassDescription("BC class for averaged irradiation.");
  return params;
}

AverageIrradiationBC::AverageIrradiationBC(const InputParameters & parameters)
  : AverageRadiationBC(parameters),
  _view_factor_calculator(getUserObject<ViewFactorCalculator>("view_factor_calculator"))
{
}

Real
AverageIrradiationBC::computeQpResidual()
{
  // Real T4_avg = _avg_rad_flux_helper.getResidual(*_current_elem, _current_side);
  // return _test[_i][_qp] * _sigma * T4_avg;
  return 0.0;
}

Real
AverageIrradiationBC::computeQpJacobian()
{
  // Real T4_avg_dT = _avg_rad_flux_helper.getJacobian(*_current_elem, _current_side, _j);
  // return _test[_i][_qp] * _sigma * T4_avg_dT;
  return 0.0;
}
