#include "SurfaceID.h"
#include <limits>
#include <assert.h>


SurfaceID::SurfaceID(libMesh::unique_id_type eid, unsigned int sid)
: elem_id(eid),
  side_id(sid)
{
  assert((10 * elem_id) < std::numeric_limits<size_t>::max());
  assert(side_id < 10);
}

bool
SurfaceID::operator==(const SurfaceID & other) const
{
  return (elem_id == other.elem_id) && (side_id == other.side_id);
}

std::size_t
SurfaceIDHash::operator()(const SurfaceID & id) const
{
  return (10 * id.elem_id) + id.side_id;
}
