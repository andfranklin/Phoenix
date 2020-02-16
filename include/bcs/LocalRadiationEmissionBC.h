#pragma once

#include "RadiationBC.h"

class LocalRadiationEmissionBC;

template <>
InputParameters validParams<LocalRadiationEmissionBC>();

/**
 * Localized radiation emission boundary condition.
**/
class LocalRadiationEmissionBC : public RadiationBC
{
public:
  static InputParameters validParams();
  LocalRadiationEmissionBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
};
