#pragma once

#include "AverageRadiationBC.h"

class AverageRadiationEmissionBC;

template <>
InputParameters validParams<AverageRadiationEmissionBC>();

/**
 * Average radiation emission boundary condition.
**/
class AverageRadiationEmissionBC : public AverageRadiationBC
{
public:
  static InputParameters validParams();
  AverageRadiationEmissionBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
};
