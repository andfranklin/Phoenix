#pragma once

#include "RadiationBC.h"

class AverageRadiationEmissionBC;

template <>
InputParameters validParams<AverageRadiationEmissionBC>();

/**
 * Average radiation emission boundary condition.
**/
class AverageRadiationEmissionBC : public RadiationBC
{
public:
  static InputParameters validParams();
  AverageRadiationEmissionBC(const InputParameters & parameters);

  virtual void computeResidual() override;
  virtual void computeJacobian() override;

protected:
  Real _T4_avg;
  DenseVector<Real> _T4_avg_dT;

  virtual void computeResidualSetup();
  virtual void computeJacobianSetup();

  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
};
