#pragma once

#include "IntegratedBC.h"
#include "RadiationBCInterface.h"

class LocalRadiationEmissionBC;

template <>
InputParameters validParams<LocalRadiationEmissionBC>();

/**
 * Localized radiation emission boundary condition.
**/
class LocalRadiationEmissionBC : public IntegratedBC,
                                 public RadiationBCInterface
{
public:
  static InputParameters validParams();
  LocalRadiationEmissionBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
};
