#pragma once

#include "InputParameters.h"

class RadiationBCInterface;
class MooseMesh;

template <>
InputParameters validParams<RadiationBCInterface>();


class RadiationBCInterface
{
public:
  RadiationBCInterface(const MooseObject * moose_object);

  static InputParameters validParams();

protected:
  Real _sigma;
};
