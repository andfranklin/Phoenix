#pragma once

#include "AverageRadiationBC.h"
#include "ViewFactorCalculator.h"

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
  const ViewFactorCalculator & _view_factor_calculator;

  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
};
