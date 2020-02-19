#pragma once

#include "IntegratedBC.h"
#include "RadiationBCInterface.h"
#include "AverageRadiationBCInterface.h"

class AverageRadiationEmissionBC;

template <>
InputParameters validParams<AverageRadiationEmissionBC>();

/**
 * Average radiation emission boundary condition.
**/
class AverageRadiationEmissionBC : public IntegratedBC,
                                   public RadiationBCInterface,
                                   public AverageRadiationBCInterface
{
public:
  static InputParameters validParams();
  AverageRadiationEmissionBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
};
