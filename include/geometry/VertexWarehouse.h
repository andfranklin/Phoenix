#ifndef VERTEXWAREHOUSE_H
#define VERTEXWAREHOUSE_H

#include "libmesh/quadrature.h"
#include "Vertex.h"
#include <map>

// Uniquely maps all libMesh nodes into a CGAL vertex. See Vertex.h.

namespace Geom
{

template <class T>
class VertexWarehouse
{
public:
  VertexWarehouse() {}
  virtual ~VertexWarehouse() {}
  const std::shared_ptr<const Vertex<T>> getVertex(const libMesh::Node * node);
  unsigned int size() { return _vertices.size(); }

protected:
  using id_t = libMesh::unique_id_type;
  using map_t = std::map<const libMesh::Node *, std::shared_ptr<const Vertex<T>>>;
  using map_iter_t = typename map_t::iterator;

  map_t _vertices;
};

template <class T>
const std::shared_ptr<const Vertex<T>>
VertexWarehouse<T>::getVertex(const libMesh::Node * node)
{
  map_iter_t vertex_iter = this->_vertices.find(node);
  if (vertex_iter != this->_vertices.end())
    return vertex_iter->second;
  else
  {
    std::shared_ptr<const Vertex<T>> vertex = std::make_shared<const Vertex<T>>(node);
    this->_vertices[node] = vertex;
    return vertex;
  }
}
}

#endif /* VERTEXWAREHOUSE_H */