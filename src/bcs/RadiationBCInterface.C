#include "RadiationBCInterface.h"

defineLegacyParams(RadiationBCInterface);

InputParameters
RadiationBCInterface::validParams()
{
  InputParameters params = emptyInputParameters();
  params.addParam<Real>("sigma", 5.670374419e-8, "The Stefan-Boltzmann constant. Default value is in units of [W / m^2 K^4].");
  return params;
}

RadiationBCInterface::RadiationBCInterface(const MooseObject * moose_object)
  : _sigma(moose_object->getParamTempl<Real>("sigma"))
{
}
