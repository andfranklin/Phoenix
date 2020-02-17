#pragma once

#include "RadiationBC.h"
#include "AverageRadiationFluxHelper.h"

class AverageRadiationEmissionBC;

template <>
InputParameters validParams<AverageRadiationEmissionBC>();

/**
 * Average radiation emission boundary condition.
**/
class AverageRadiationEmissionBC : public RadiationBC
{
public:
  static InputParameters validParams();
  AverageRadiationEmissionBC(const InputParameters & parameters);

protected:
  const AverageRadiationFluxHelper & _avg_rad_flux_helper;

  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
};
