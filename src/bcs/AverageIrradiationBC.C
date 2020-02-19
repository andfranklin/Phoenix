#include "AverageIrradiationBC.h"
#include "SurfaceID.h"

registerMooseObject("PhoenixApp", AverageIrradiationBC);

defineLegacyParams(AverageIrradiationBC);

InputParameters
AverageIrradiationBC::validParams()
{
  InputParameters params = IntegratedBC::validParams();
  params += RadiationBCInterface::validParams();
  params += AverageRadiationBCInterface::validParams();

  params.addRequiredParam<UserObjectName>("view_factor_calculator",
                                          "Interface to the view factors.");

  params.addClassDescription("BC class for averaged irradiation.");
  return params;
}

AverageIrradiationBC::AverageIrradiationBC(const InputParameters & parameters)
  : IntegratedBC(parameters),
    RadiationBCInterface(this),
    AverageRadiationBCInterface(this),
    _view_factor_calculator(getUserObject<ViewFactorCalculator>("view_factor_calculator"))
{
}

void
AverageIrradiationBC::computeElemAvgIrradiation()
{
  SurfaceID to_surf_id = {*_current_elem, _current_side};
  const std::vector<SurfaceID> & from_surf_ids = _view_factor_calculator.getConnectedSurfaceIDs(to_surf_id);

  _elem_avg_irradiation = 0.0;
  for (const SurfaceID & from_surf_id : from_surf_ids)
  {
    Real vf = _view_factor_calculator.getViewFactor(from_surf_id, to_surf_id);
    Real T4_avg = _avg_rad_flux_helper.getResidual(from_surf_id);
    _elem_avg_irradiation += vf * T4_avg;
  }

  _elem_avg_irradiation = -1.0 * _sigma * _elem_avg_irradiation;
}

Real
AverageIrradiationBC::computeQpResidual()
{
  return _test[_i][_qp] * _elem_avg_irradiation;
}

void
AverageIrradiationBC::computeResidual()
{
  computeElemAvgIrradiation();
  IntegratedBC::computeResidual();
}
