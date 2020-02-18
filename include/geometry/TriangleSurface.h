#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include "GeometryUtils.h"
#include "Surface.h"

// The triangle surface class.

namespace Geom
{

template <class T>
class TriangleSurface : public Surface<T>
{
public:
  TriangleSurface(SurfaceID surface_id,
                  const std::shared_ptr<const Vertex<T>> a,
                  const std::shared_ptr<const Vertex<T>> b,
                  const std::shared_ptr<const Vertex<T>> c);
  virtual ~TriangleSurface() {}

  virtual bool definitiveIntersection(const Segment<T> & other) override;

  virtual typename T::Point_3 getCentroid() override;

protected:
  libMesh::Real _abs_jacobian;
  typename T::Triangle_3 _triangle;

protected:
  virtual typename T::Point_3 getSurfaceLocation(libMesh::Real X, libMesh::Real Y) override;
  virtual libMesh::Real getAbsJacobian(libMesh::Real X, libMesh::Real Y) override;

  virtual void calculateArea() override;
  virtual void calculateNormal() override;
};

template <class T>
TriangleSurface<T>::TriangleSurface(SurfaceID surface_id,
                                    const std::shared_ptr<const Vertex<T>> a,
                                    const std::shared_ptr<const Vertex<T>> b,
                                    const std::shared_ptr<const Vertex<T>> c)
  : Surface<T>(surface_id, a, b, c),
  _triangle(buildTriangle<T>(this->_a, this->_b, this->_c))
{
  this->initializeSurface();
  _abs_jacobian = 2.0 * this->getArea();
}

template <class T>
void
TriangleSurface<T>::calculateArea()
{
  this->_area = getArea<T>(_triangle);
}

template <class T>
void
TriangleSurface<T>::calculateNormal()
{
  this->_normal = getNormal<T>(_triangle) / 2;
}

template <class T>
bool
TriangleSurface<T>::definitiveIntersection(const Segment<T> & other)
{
  return CGAL::do_intersect(other.segment(), this->_triangle);
}

template <class T>
typename T::Point_3
TriangleSurface<T>::getSurfaceLocation(libMesh::Real X, libMesh::Real Y)
{
  libmesh_assert_msg((X >= 0 && X <= 1) && (Y >= 0 && Y <= 1) && (X + Y <= 1),
                     "Point specified outside of triangle's domain.");

  typename T::Vector_3 return_locus;
  return_locus = (1 - X - Y) * this->_a->locus();
  return_locus += X * this->_b->locus();
  return_locus += Y * this->_c->locus();
  return ORIGIN + return_locus;
}

template <class T>
libMesh::Real TriangleSurface<T>::getAbsJacobian(libMesh::Real /*X*/, libMesh::Real /*Y*/)
{
  return _abs_jacobian;
}

template <class T>
typename T::Point_3
TriangleSurface<T>::getCentroid()
{
  if (this->_centroid_not_set)
    this->_centroid = getSurfaceLocation(0.5, 0.5);
  return this->_centroid;
}

}

#endif /* TRIANGLESURFACE_H */
