#pragma once

#include "IntegratedBC.h"

class RadiationBC;

template <>
InputParameters validParams<RadiationBC>();

/**
 * Base class for radiation boundary conditions.
**/
class RadiationBC : public IntegratedBC
{
public:
  static InputParameters validParams();
  RadiationBC(const InputParameters & parameters);

protected:
  Real _sigma;
};
