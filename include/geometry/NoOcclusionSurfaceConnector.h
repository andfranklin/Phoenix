#ifndef NOOCCLUSIONSURFACECONNECTOR_H
#define NOOCCLUSIONSURFACECONNECTOR_H

#include "SurfaceConnector.h"

namespace Geom
{

// Assumes that there is no occlusion. All surfaces that face
// eachother "communicate". This option is valid if the problem
// consists of objects that do not self-occlude and do not occlude one
// another. If users know this before hand then a lot of computation
// time can be saved.
template <class BaseKernel, class QuadKernel, class CollisionKernel>
class NoOcclusionSurfaceConnector : public SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>
{
public:
  using SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::SurfaceConnector;

protected:
  virtual bool isOccluded(const Segment<BaseKernel> & path) override;
};

template <class BaseKernel, class QuadKernel, class CollisionKernel>
bool
NoOcclusionSurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::isOccluded(
    const Segment<BaseKernel> & /*path*/)
{
  return false;
}
}

#endif /* NOOCCLUSIONSURFACECONNECTOR_H */