#pragma once

#include "SurfaceID.h"
#include <vector>

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

  const SurfaceID & getID() const { return _surface_id; }

  void connect(const SurfaceBase & surf) { connect(surf.getID()); }
  void clearConnections() { _connected_surfaces.erase(_connected_surfaces.begin(), _connected_surfaces.end()); }
  const std::vector<SurfaceID> & getConnections() const { return _connected_surfaces; }

  virtual ~SurfaceBase() {}

protected:
  SurfaceID _surface_id;
  std::vector<SurfaceID> _connected_surfaces;

  void connect(const SurfaceID & surface_id) { _connected_surfaces.emplace_back(surface_id); }
};

}
