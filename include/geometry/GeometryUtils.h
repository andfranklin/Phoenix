#ifndef GEOMETRYUTILS_H
#define GEOMETRYUTILS_H

#include "libmesh/libmesh_common.h"
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Cartesian_converter.h>
#include <cmath>

namespace Geom
{

const auto ORIGIN = CGAL::ORIGIN;

using Apprx = CGAL::Simple_cartesian<libMesh::Real>;
using Exact = CGAL::Exact_predicates_exact_constructions_kernel;

template <class T>
inline libMesh::Real
toReal(const T & number)
{
  return number;
}

template <>
inline libMesh::Real
toReal(const typename Exact::FT & number)
{
  return number.exact().to_double();
}

template <class T>
inline libMesh::Real
apprxSqrt(const typename T::FT & number)
{
  return sqrt(toReal<typename T::FT>(number));
}

template <class T>
inline typename T::Vector_3
normalize(const typename T::Vector_3 & vect)
{
  return vect / apprxSqrt<T>(vect.squared_length());
}

template <class T>
inline bool
isHeadOn(const typename T::Vector_3 & surface_normal, const typename T::Vector_3 & ray_direction)
{
  return CGAL::angle(surface_normal, ray_direction) == CGAL::OBTUSE;
}

template <class T>
inline typename T::Plane_3
getPlane(const typename T::Triangle_3 & triangle)
{
  return triangle.supporting_plane();
}

template <class T>
inline typename T::Vector_3
getNormal(const typename T::Triangle_3 & triangle)
{
  return getPlane<T>(triangle).orthogonal_vector();
}

template <class T>
inline libMesh::Real
getArea(const typename T::Triangle_3 & triangle)
{
  return apprxSqrt<T>(triangle.squared_area());
}

// rawViewFactorKernel assumes that the surface normals are directed
// twords eachother and the path is unoccluded. It is up to the users to
// ensure that this is true if they want to receive correct results. If
// the normals are not directed twords eachother then the returned value
// will be negative.

// This function assume that the normals are unit vectors. If the user
// supplies normals that are not unit vectors they need to divide the
// return value the magnitude of each normal.

template <class T>
inline libMesh::Real
rawViewFactorKernel(const typename T::Vector_3 & path,
                    const typename T::Vector_3 & from_normal,
                    const typename T::Vector_3 & to_normal)
{
  typename T::FT numer = -1 * (from_normal * path) * (to_normal * path);
  typename T::FT path_length_sqrd = path.squared_length();
  typename T::FT denom = M_PI * path_length_sqrd * path_length_sqrd;
  return toReal<typename T::FT>(numer / denom);
}
}

#endif /* GEOMETRYUTILS_H */