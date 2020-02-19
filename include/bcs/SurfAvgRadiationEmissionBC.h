#pragma once

#include "IntegratedBC.h"
#include "RadiationBCInterface.h"
#include "AverageRadiationBCInterface.h"

class SurfAvgRadiationEmissionBC;

template <>
InputParameters validParams<SurfAvgRadiationEmissionBC>();

/**
 * Average radiation emission boundary condition.
**/
class SurfAvgRadiationEmissionBC : public IntegratedBC,
                                   public RadiationBCInterface,
                                   public AverageRadiationBCInterface
{
public:
  static InputParameters validParams();
  SurfAvgRadiationEmissionBC(const InputParameters & parameters);

protected:
  virtual void computeResidual() override;

  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  Real _T4_avg;

};
