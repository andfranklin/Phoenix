#pragma once

#include "libmesh/id_types.h"
#include "libmesh/elem.h"

class SurfaceID
{
public:
  SurfaceID(libMesh::unique_id_type elem_id, unsigned int side_id);
  SurfaceID(const libMesh::Elem & elem, unsigned int side_id);
  virtual ~SurfaceID() {}

  bool operator==(const SurfaceID & other) const;

protected:
  libMesh::unique_id_type elem_id;
  unsigned int side_id;

  friend class SurfaceIDHash;
};

struct SurfaceIDHash {
  std::size_t operator()(const SurfaceID & id) const;
};
