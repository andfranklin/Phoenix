#ifndef QUADRILATERALSURFACE_H
#define QUADRILATERALSURFACE_H

#include "GeometryUtils.h"
#include "SurfaceBase.h"

// The quadrilateral surface class.

namespace Geom
{

template <class T>
class QuadrilateralSurface : public SurfaceBase<T>
{
public:
  QuadrilateralSurface(const std::shared_ptr<const Vertex<T>> a,
                       const std::shared_ptr<const Vertex<T>> b,
                       const std::shared_ptr<const Vertex<T>> c,
                       const std::shared_ptr<const Vertex<T>> d);
  virtual ~QuadrilateralSurface() {}

  virtual bool definitiveIntersection(const Segment<T> & other) override;

  virtual typename T::Point_3 getCentroid() override;

protected:
  // because of the potential for floating point errors, d is projected
  // onto the same plane defined by a, b, and c. A check should be done
  // in the constructor to verify that d is sufficiently close to _d.
  typename T::Triangle_3 _triangle_1;
  std::shared_ptr<const Vertex<T>> _d;
  typename T::Triangle_3 _triangle_2;

protected:
  virtual void calculateArea() override;
  virtual void calculateNormal() override;
  virtual void formBBox() override;

  libMesh::Real nodal_func_1(libMesh::Real X, libMesh::Real Y);
  libMesh::Real nodal_func_1_dX(libMesh::Real Y);
  libMesh::Real nodal_func_1_dY(libMesh::Real X);

  libMesh::Real nodal_func_2(libMesh::Real X, libMesh::Real Y);
  libMesh::Real nodal_func_2_dX(libMesh::Real Y);
  libMesh::Real nodal_func_2_dY(libMesh::Real X);

  libMesh::Real nodal_func_3(libMesh::Real X, libMesh::Real Y);
  libMesh::Real nodal_func_3_dX(libMesh::Real Y);
  libMesh::Real nodal_func_3_dY(libMesh::Real X);

  libMesh::Real nodal_func_4(libMesh::Real X, libMesh::Real Y);
  libMesh::Real nodal_func_4_dX(libMesh::Real Y);
  libMesh::Real nodal_func_4_dY(libMesh::Real X);

protected:
  virtual typename T::Point_3 getSurfaceLocation(libMesh::Real X, libMesh::Real Y) override;
  virtual libMesh::Real getAbsJacobian(libMesh::Real X, libMesh::Real Y) override;
};

template <class T>
QuadrilateralSurface<T>::QuadrilateralSurface(const std::shared_ptr<const Vertex<T>> a,
                                              const std::shared_ptr<const Vertex<T>> b,
                                              const std::shared_ptr<const Vertex<T>> c,
                                              const std::shared_ptr<const Vertex<T>> d)
  : SurfaceBase<T>(a, b, c),
    _triangle_1(buildTriangle<T>(this->_a, this->_b, this->_c)),
    _d(std::make_shared<const Vertex<T>>(d->projectOnto(_triangle_1))),
    _triangle_2(buildTriangle<T>(this->_a, this->_c, this->_d))
{
  this->initializeSurfaceBase();
}

template <class T>
void
QuadrilateralSurface<T>::formBBox()
{
  this->_bbox += this->_d->bbox();
}

template <class T>
void
QuadrilateralSurface<T>::calculateNormal()
{
  typename T::Vector_3 normal_1(getNormal<T>(_triangle_1));
  typename T::Vector_3 normal_2(getNormal<T>(_triangle_2));
  CGAL::Angle angle(CGAL::angle(normal_1, normal_2));

  switch (angle)
  {
    case CGAL::RIGHT:
    case CGAL::OBTUSE:
      std::cout << "Triangle degenerate" << std::endl;
    default:
      this->_normal = (normal_1 + normal_2) / 2;
  }
}

template <class T>
void
QuadrilateralSurface<T>::calculateArea()
{
  this->_area = getArea<T>(_triangle_1) + getArea<T>(_triangle_2);
}

template <class T>
bool
QuadrilateralSurface<T>::definitiveIntersection(const Segment<T> & other)
{
  return CGAL::do_intersect(other.segment(), this->_triangle_1) ||
         CGAL::do_intersect(other.segment(), this->_triangle_2);
}

template <class T>
inline libMesh::Real
QuadrilateralSurface<T>::nodal_func_1(libMesh::Real X, libMesh::Real Y)
{
  return 0.25 * (1.0 - X) * (1.0 - Y);
}

template <class T>
inline libMesh::Real
QuadrilateralSurface<T>::nodal_func_1_dX(libMesh::Real Y)
{
  return -0.25 * (1.0 - Y);
}

template <class T>
inline libMesh::Real
QuadrilateralSurface<T>::nodal_func_1_dY(libMesh::Real X)
{
  return -0.25 * (1.0 - X);
}

template <class T>
inline libMesh::Real
QuadrilateralSurface<T>::nodal_func_2(libMesh::Real X, libMesh::Real Y)
{
  return 0.25 * (1.0 + X) * (1.0 - Y);
}

template <class T>
inline libMesh::Real
QuadrilateralSurface<T>::nodal_func_2_dX(libMesh::Real Y)
{
  return 0.25 * (1.0 - Y);
}

template <class T>
inline libMesh::Real
QuadrilateralSurface<T>::nodal_func_2_dY(libMesh::Real X)
{
  return -0.25 * (1.0 + X);
}

template <class T>
inline libMesh::Real
QuadrilateralSurface<T>::nodal_func_3(libMesh::Real X, libMesh::Real Y)
{
  return 0.25 * (1.0 + X) * (1.0 + Y);
}

template <class T>
inline libMesh::Real
QuadrilateralSurface<T>::nodal_func_3_dX(libMesh::Real Y)
{
  return 0.25 * (1.0 + Y);
}

template <class T>
inline libMesh::Real
QuadrilateralSurface<T>::nodal_func_3_dY(libMesh::Real X)
{
  return 0.25 * (1.0 + X);
}

template <class T>
inline libMesh::Real
QuadrilateralSurface<T>::nodal_func_4(libMesh::Real X, libMesh::Real Y)
{
  return 0.25 * (1.0 - X) * (1.0 + Y);
}

template <class T>
inline libMesh::Real
QuadrilateralSurface<T>::nodal_func_4_dX(libMesh::Real Y)
{
  return -0.25 * (1.0 + Y);
}

template <class T>
inline libMesh::Real
QuadrilateralSurface<T>::nodal_func_4_dY(libMesh::Real X)
{
  return 0.25 * (1.0 - X);
}

template <class T>
typename T::Point_3
QuadrilateralSurface<T>::getSurfaceLocation(libMesh::Real X, libMesh::Real Y)
{
  libmesh_assert_msg((X >= -1 && X <= 1) && (Y >= -1 && Y <= 1),
                     "Point specified outside of quadrilateral's domain.");

  typename T::Vector_3 return_locus;

  return_locus = nodal_func_1(X, Y) * this->_a->locus();
  return_locus += nodal_func_2(X, Y) * this->_b->locus();
  return_locus += nodal_func_3(X, Y) * this->_c->locus();
  return_locus += nodal_func_4(X, Y) * this->_d->locus();

  return ORIGIN + return_locus;
}

template <class T>
libMesh::Real
QuadrilateralSurface<T>::getAbsJacobian(libMesh::Real X, libMesh::Real Y)
{
  libmesh_assert_msg((X >= -1 && X <= 1) && (Y >= -1 && Y <= 1),
                     "Point specified outside of quadrilateral's domain.");

  typename T::Vector_3 partial_with_X;
  partial_with_X = nodal_func_1_dX(Y) * this->_a->locus();
  partial_with_X += nodal_func_2_dX(Y) * this->_b->locus();
  partial_with_X += nodal_func_3_dX(Y) * this->_c->locus();
  partial_with_X += nodal_func_4_dX(Y) * this->_d->locus();

  typename T::Vector_3 partial_with_Y;
  partial_with_Y = nodal_func_1_dY(X) * this->_a->locus();
  partial_with_Y += nodal_func_2_dY(X) * this->_b->locus();
  partial_with_Y += nodal_func_3_dY(X) * this->_c->locus();
  partial_with_Y += nodal_func_4_dY(X) * this->_d->locus();

  typename T::Vector_3 jacobian_vect(CGAL::cross_product(partial_with_X, partial_with_Y));

  return sqrt(toReal<typename T::FT>(jacobian_vect.squared_length()));
}

template <class T>
typename T::Point_3
QuadrilateralSurface<T>::getCentroid()
{
  if (this->_centroid_not_set)
    this->_centroid = getSurfaceLocation(0.0, 0.0);
  return this->_centroid;
}

}

#endif /* QUADRILATERALSURFACE_H */