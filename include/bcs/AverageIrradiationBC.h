#pragma once

#include "AverageRadiationBC.h"

class AverageIrradiationBC;

template <>
InputParameters validParams<AverageIrradiationBC>();

/**
 * Average irradiation boundary condition.
**/
class AverageIrradiationBC : public AverageRadiationBC
{
public:
  static InputParameters validParams();
  AverageIrradiationBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
};
