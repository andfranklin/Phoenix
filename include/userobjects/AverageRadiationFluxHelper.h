#pragma once

#include "ShapeSideUserObject.h"
#include "MooseVariableInterface.h"
#include "SurfaceID.h"
#include <unordered_map>


class AverageRadiationFluxHelper;

template <>
InputParameters validParams<AverageRadiationFluxHelper>();

class AverageRadiationFluxHelper : public ShapeSideUserObject,
                                   public MooseVariableInterface<Real>
{
public:
  AverageRadiationFluxHelper(const InputParameters & parameters);
  virtual ~AverageRadiationFluxHelper() {};

  virtual void initialize() override {};
  virtual void execute() override;
  virtual void threadJoin(const UserObject & /*y*/) override {};
  virtual void finalize() override {};

  Real getResidual(const Elem & elem, unsigned int side) const;
  Real getJacobian(const Elem & elem, unsigned int side, unsigned int j) const;

protected:
  Real computeQpResidual();
  void computeResidual();

  Real computeQpJacobian();
  void computeJacobian();

  unsigned int _qp;

  virtual void executeJacobian(unsigned int /*jvar*/) override {};

  const VariableValue & _temp;
  const VariableGradient & _grad_temp;

private:
  std::unordered_map<SurfaceID, Real, SurfaceIDHash> _element_surface_to_res;
  std::unordered_map<SurfaceID, DenseVector<Real>, SurfaceIDHash> _element_surface_to_jac;
};
