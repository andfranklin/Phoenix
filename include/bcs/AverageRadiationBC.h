#pragma once

#include "RadiationBC.h"
#include "AverageRadiationFluxHelper.h"

class AverageRadiationBC;

template <>
InputParameters validParams<AverageRadiationBC>();

/**
 * Average radiation boundary condition base class.
**/
class AverageRadiationBC : public RadiationBC
{
public:
  static InputParameters validParams();
  AverageRadiationBC(const InputParameters & parameters);

protected:
  const AverageRadiationFluxHelper & _avg_rad_flux_helper;
};
