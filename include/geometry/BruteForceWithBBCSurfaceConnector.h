#pragma once

#include "SurfaceConnector.h"

namespace Geom
{

// Does a brute force search through the surface mesh to determine
// if an integration path is occluded. The integration paths are first
// compared with the bounding boxes of the surfaces before the
// definitive collision detection is applied.
template <class BaseKernel, class QuadKernel, class CollisionKernel>
class BruteForceWithBBCSurfaceConnector
    : public SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>
{
public:
  using SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::SurfaceConnector;

protected:
  virtual bool isOccluded(const Segment<BaseKernel> & path) override;
};

template <class BaseKernel, class QuadKernel, class CollisionKernel>
bool
BruteForceWithBBCSurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::isOccluded(
    const Segment<BaseKernel> & path)
{
  for (const std::shared_ptr<Surface<BaseKernel>> & surface :
       this->_surface_warehouse.getSurfaces())
    if (surface->shortCircuitIntersection(path))
      if ((surface != this->_from_surf) && (surface != this->_to_surf))
        return true;

  return false;
}
}
