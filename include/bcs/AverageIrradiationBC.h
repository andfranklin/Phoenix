#pragma once

#include "IntegratedBC.h"
#include "RadiationBCInterface.h"
#include "AverageRadiationBCInterface.h"
#include "ViewFactorCalculator.h"

class AverageIrradiationBC;

template <>
InputParameters validParams<AverageIrradiationBC>();

/**
 * Average irradiation boundary condition.
**/
class AverageIrradiationBC : public IntegratedBC,
                             public RadiationBCInterface,
                             public AverageRadiationBCInterface
{
public:
  static InputParameters validParams();
  AverageIrradiationBC(const InputParameters & parameters);

protected:
  const ViewFactorCalculator & _view_factor_calculator;
  Real _elem_avg_irradiation;

  virtual Real computeQpResidual() override;
  virtual void computeResidual() override;

  void computeElemAvgIrradiation();
};
