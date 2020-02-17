#ifndef AVERAGERADIATIONFLUXHELPER_H
#define AVERAGERADIATIONFLUXHELPER_H

#include "ShapeSideUserObject.h"
#include "MooseVariableInterface.h"
#include <unordered_map>
#include <tuple>


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
  // (elem's unique_id, elem's side)
  using SideElemKey = std::tuple<unique_id_type, unsigned int>;

  std::map<SideElemKey, Real> _element_surface_to_res;
  std::map<SideElemKey, DenseVector<Real>> _element_surface_to_jac;
};

#endif /* AVERAGERADIATIONFLUXHELPER_H */
