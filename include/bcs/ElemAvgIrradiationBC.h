#pragma once

#include "IntegratedBC.h"
#include "RadiationBCInterface.h"
#include "AverageRadiationBCInterface.h"
#include "ViewFactorCalculator.h"

class ElemAvgIrradiationBC;

template <>
InputParameters validParams<ElemAvgIrradiationBC>();

/**
 * Average irradiation boundary condition.
**/
class ElemAvgIrradiationBC : public IntegratedBC,
                             public RadiationBCInterface,
                             public AverageRadiationBCInterface
{
public:
  static InputParameters validParams();
  ElemAvgIrradiationBC(const InputParameters & parameters);

protected:
  const ViewFactorCalculator & _view_factor_calculator;
  Real _elem_avg_irradiation;

  virtual Real computeQpResidual() override;
  virtual void computeResidual() override;

  void computeElemAvgIrradiation();
};
