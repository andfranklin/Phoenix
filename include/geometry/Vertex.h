#ifndef VERTEX_H
#define VERTEX_H

#include "GeometryUtils.h"
#include "libmesh/node.h"

// Used for the reconstruction of a surface mesh in CGAL.
// The vertex can be thought of as an arbitrary point. The data structure
// consists of a point (T::Point_3) and the locus (T::Vector_3) <add ref>.
// This distinction is necessary because it is meaningful in CGAL. Encapsulation
// in this data structure has the additional benefit of uniquely storing
// and identifying all verticies on a surface mesh in the VertexWarehouse. 

namespace Geom
{

template <class T>
class Vertex
{
public:
  Vertex(const libMesh::Node * node);
  Vertex(const typename T::Point_3 & point);
  virtual ~Vertex() {}

  const typename T::Point_3 & point() const { return _point; }
  const typename T::Vector_3 & locus() const { return _locus; }

  CGAL::Bbox_3 bbox() const { return _point.bbox(); }

  Vertex<T> projectOnto(const typename T::Plane_3 & plane) const;
  Vertex<T> projectOnto(const typename T::Triangle_3 & triangle) const;

protected:
  const typename T::Point_3 _point;
  const typename T::Vector_3 _locus;
};

template <class T>
Vertex<T>::Vertex(const libMesh::Node * node)
  : _point(node->operator()(0), node->operator()(1), node->operator()(2)), _locus(_point - ORIGIN)
{
}

template <class T>
Vertex<T>::Vertex(const typename T::Point_3 & point) : _point(point), _locus(_point - ORIGIN)
{
}

template <class T>
Vertex<T>
Vertex<T>::projectOnto(const typename T::Plane_3 & plane) const
{
  return plane.projection(this->_point);
}

template <class T>
Vertex<T>
Vertex<T>::projectOnto(const typename T::Triangle_3 & triangle) const
{
  return projectOnto(getPlane<T>(triangle));
}

template <class T>
inline typename T::Triangle_3
buildTriangle(const std::shared_ptr<const Vertex<T>> & a,
              const std::shared_ptr<const Vertex<T>> & b,
              const std::shared_ptr<const Vertex<T>> & c)
{
  return typename T::Triangle_3(a->point(), b->point(), c->point());
}
}

#endif /* VERTEX_H */