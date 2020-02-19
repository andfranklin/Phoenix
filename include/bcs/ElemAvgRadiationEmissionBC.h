#pragma once

#include "IntegratedBC.h"
#include "RadiationBCInterface.h"
#include "AverageRadiationBCInterface.h"

class ElemAvgRadiationEmissionBC;

template <>
InputParameters validParams<ElemAvgRadiationEmissionBC>();

/**
 * Average radiation emission boundary condition.
**/
class ElemAvgRadiationEmissionBC : public IntegratedBC,
                                   public RadiationBCInterface,
                                   public AverageRadiationBCInterface
{
public:
  static InputParameters validParams();
  ElemAvgRadiationEmissionBC(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
};
