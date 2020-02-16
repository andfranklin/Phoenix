#include "RadiationBC.h"

defineLegacyParams(RadiationBC);

InputParameters
RadiationBC::validParams()
{
  InputParameters params = IntegratedBC::validParams();
  params.addParam<Real>("sigma", 5.670374419e-8, "The Stefan-Boltzmann constant. Default value is in units of [W / m^2 K^4].");
  params.addClassDescription("This is the base BC class for radiation heat transfer.");
  return params;
}

RadiationBC::RadiationBC(const InputParameters & parameters)
  : IntegratedBC(parameters),
    _sigma(getParam<Real>("sigma"))
{
}
