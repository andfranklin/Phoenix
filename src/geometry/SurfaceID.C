#include "SurfaceID.h"
#include <iostream>
#include <limits>
#include <assert.h>

namespace Geom
{

SurfaceID::SurfaceID(libMesh::unique_id_type eid, unsigned int sid)
: elem_id(eid),
  side_id(sid)
{
  assert((10 * elem_id) < std::numeric_limits<size_t>::max());
  assert(side_id < 10);
}

size_t
SurfaceIDHash::operator()(const SurfaceID & id) const
{
  return (10 * id.elem_id) + id.side_id;
}

}
