#ifndef SURFACEBASE_H
#define SURFACEBASE_H

#include "GeometryUtils.h"
#include "VertexWarehouse.h"
#include "libmesh/quadrature.h"
#include "Segment.h"

// Abstract base class for 2D surfaces.

namespace Geom
{

template <class T>
class SurfaceBase
{
protected:
  using qp_pair_t = std::pair<const typename T::Point_3 &, const typename libMesh::Real>;

public:
  SurfaceBase(const std::shared_ptr<const Vertex<T>> a,
              const std::shared_ptr<const Vertex<T>> b,
              const std::shared_ptr<const Vertex<T>> c);
  virtual ~SurfaceBase() {}

  void init(const libMesh::QBase & quad);

  libMesh::Real getArea() const { return _area; }
  const typename T::Vector_3 & getNormal() const { return _normal; }

  const std::vector<typename T::Point_3> & getEmbeddedQps() const { return _embedded_qps; }
  const std::vector<typename libMesh::Real> & getJxWs() const { return _JxW; }
  const std::vector<qp_pair_t> & getQpPairs() const { return _qp_pairs; }

  typename T::Point_3 projectOntoPlane(const typename T::Point_3 & point);

  // definitive check that a segment intersects with the surface
  virtual bool definitiveIntersection(const Segment<T> & other) = 0;

  // first checks if the bounding boxes overlap. If they do then
  // definitiveIntersection is used. Otherwise, returns false.
  bool shortCircuitIntersection(const Segment<T> & other);

  // might not need these two methods if only flat surfaces are ever
  // assumed
  bool isHeadedOnBy(const Segment<T> & other);
  bool isIntersectedHeadOnBy(const Segment<T> & other);

  const typename T::Point_3 & qp(unsigned int i);
  const CGAL::Bbox_3 & bbox() { return _bbox; }

  typename std::vector<typename T::Point_3>::iterator begin() { return _embedded_qps.begin(); }
  typename std::vector<typename T::Point_3>::iterator end() { return _embedded_qps.end(); }

  virtual typename T::Point_3 getCentroid() = 0;

protected:
  std::shared_ptr<const Vertex<T>> _a;
  std::shared_ptr<const Vertex<T>> _b;
  std::shared_ptr<const Vertex<T>> _c;

  // quadrature info
  std::vector<typename T::Point_3> _embedded_qps;
  std::vector<typename libMesh::Real> _JxW;
  std::vector<qp_pair_t> _qp_pairs;

  typename T::Vector_3 _normal; // has a magnitude of the area.
  libMesh::Real _area;
  CGAL::Bbox_3 _bbox;

  bool _centroid_not_set;
  typename T::Point_3 _centroid;

protected:
  virtual typename T::Point_3 getSurfaceLocation(libMesh::Real X, libMesh::Real Y) = 0;
  virtual libMesh::Real getAbsJacobian(libMesh::Real X, libMesh::Real Y) = 0;

  void initializeSurfaceBase(); // should be called in constuctor of child classes
  virtual void calculateArea() = 0;
  virtual void calculateNormal() = 0;
  virtual void formBBox();
};

template <class T>
SurfaceBase<T>::SurfaceBase(const std::shared_ptr<const Vertex<T>> a,
                            const std::shared_ptr<const Vertex<T>> b,
                            const std::shared_ptr<const Vertex<T>> c)
  : _a(a),
    _b(b),
    _c(c),

    _embedded_qps(),
    _JxW(),

    _bbox(_a->bbox() + _b->bbox() + _c->bbox()),
    _centroid_not_set(true)
{
}

template <class T>
typename T::Point_3
SurfaceBase<T>::projectOntoPlane(const typename T::Point_3 & point)
{
  typename T::Plane_3 plane(_a->point(), _b->point(), _c->point());
  return plane.projection(point);
}

template <class T>
void
SurfaceBase<T>::initializeSurfaceBase()
{
  this->formBBox();
  this->calculateNormal();
  this->calculateArea();
}

template <class T>
void
SurfaceBase<T>::init(const libMesh::QBase & quad)
{
  if (_embedded_qps.size() != 0)
    _embedded_qps.clear();
  _embedded_qps.reserve(quad.n_points());

  _JxW.clear();
  _JxW.reserve(quad.n_points());

  _qp_pairs.clear();
  _qp_pairs.reserve(quad.n_points());

  unsigned int indx = 0;
  for (const auto & qp : quad.get_points())
  {
    _embedded_qps.push_back(getSurfaceLocation(qp(0), qp(1)));
    _JxW.push_back(getAbsJacobian(qp(0), qp(1)) * quad.w(indx));
    _qp_pairs.push_back(qp_pair_t(_embedded_qps[indx], _JxW[indx]));
    ++indx;
  }
}

template <class T>
const typename T::Point_3 &
SurfaceBase<T>::qp(unsigned int i)
{
  return _embedded_qps[i];
}

template <class T>
void
SurfaceBase<T>::formBBox()
{
}

template <class T>
bool
SurfaceBase<T>::shortCircuitIntersection(const Segment<T> & other)
{
  if (CGAL::do_overlap(_bbox, other.bbox()))
    return this->definitiveIntersection(other);
  else
    return false;
}

template <class T>
bool
SurfaceBase<T>::isHeadedOnBy(const Segment<T> & other)
{
  return isHeadOn<T>(_normal, other.vector());
}

template <class T>
bool
SurfaceBase<T>::isIntersectedHeadOnBy(const Segment<T> & other)
{
  if (shortCircuitIntersection(other))
    return isHeadOn<T>(_normal, other.vector());
  else
    return false;
}
}

#endif /* SURFACEBASE_H */
