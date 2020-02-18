#pragma once

#include "SurfaceID.h"

namespace Geom
{

// Base class for all surfaces.
//
// Responsible for managing radiation connections between surfaces and
// identification.

class SurfaceBase
{
public:
  SurfaceBase(const SurfaceID & surface_id);
  virtual ~SurfaceBase() {}

protected:
  SurfaceID _surface_id;
};

}
