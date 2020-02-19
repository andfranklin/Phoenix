#pragma once

#include "InputParameters.h"
#include "BoundaryCondition.h"
#include "AverageRadiationFluxHelper.h"

class AverageRadiationBCInterface;

template <>
InputParameters validParams<AverageRadiationBCInterface>();

/**
 * Average radiation boundary condition interface.
**/
class AverageRadiationBCInterface
{
public:
  static InputParameters validParams();
  AverageRadiationBCInterface(BoundaryCondition * bc);

protected:
  const AverageRadiationFluxHelper & _avg_rad_flux_helper;
};
