#ifndef SURFACEID_H
#define SURFACEID_H

#include "libmesh/id_types.h"

namespace Geom
{

class SurfaceID
{
public:
  SurfaceID(libMesh::unique_id_type elem_id, unsigned int side_id);
  virtual ~SurfaceID() {}

  const libMesh::unique_id_type elem_id;
  const unsigned int side_id;
};

class SurfaceIDHash {
public:
    size_t operator()(const SurfaceID & id) const;
};

}

#endif /* SURFACEID_H */
