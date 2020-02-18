#ifndef SEGMENT_H
#define SEGMENT_H

#include "GeometryUtils.h"

// Represents a line segment embedded in 3-dimensional space.
// Defined by two points, typically on the surface of some geometry
// between which the view factors are being calculated.

namespace Geom
{

template <class T>
class Surface;

template <class T>
class Segment
{
public:
  Segment(const typename T::Point_3 & source, const typename T::Point_3 & target);
  virtual ~Segment() {}

  const typename T::Segment_3 & segment() const { return _segment; }
  const typename T::Vector_3 & vector() const { return _vector; }
  const typename CGAL::Bbox_3 & bbox() const { return _bbox; }
  bool isHeadOnWith(const Surface<T> * test_surface);

protected:
  typename T::Segment_3 _segment;
  typename T::Vector_3 _vector;
  typename CGAL::Bbox_3 _bbox;
};

template <class T>
Segment<T>::Segment(const typename T::Point_3 & source, const typename T::Point_3 & target)
  : _segment(source, target), _vector(_segment.to_vector()), _bbox(_segment.bbox())
{
}

template <class T>
bool
Segment<T>::isHeadOnWith(const Surface<T> * test_surface)
{
  return isHeadOn<T>(test_surface->getNormal(), _vector);
}
}

#endif /* SEGMENT_H */