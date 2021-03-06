#pragma once

#include "SurfaceConnectorBase.h"
#include "SurfaceWarehouse.h"
#include "SymmetricDualMap.h"

namespace Geom
{

template <class BaseKernel, class QuadKernel, class CollisionKernel>
class SurfaceConnector : public SurfaceConnectorBase
{
public:
  using SurfacePointer = std::shared_ptr<Surface<BaseKernel>>;

  SurfaceConnector();
  virtual ~SurfaceConnector() {}

  virtual libMesh::Real getViewFactor(const SurfaceID & from_surf_id, const SurfaceID & to_surf_id) override;
  virtual libMesh::Real getArea(const SurfaceID & surf_id) override;
  virtual const std::vector<SurfaceID> & getConnectedSurfaceIDs(const SurfaceID & surf_id) override;

  virtual void buildSurface(const SurfaceID & surf_id, const libMesh::Node * a, const libMesh::Node * b, const libMesh::Node * c) override;
  virtual void buildSurface(const SurfaceID & surf_id, const libMesh::Node * a, const libMesh::Node * b, const libMesh::Node * c, const libMesh::Node * d) override;

  virtual void setQuadratureType(const libMesh::QuadratureType & quadrature_type) override;
  virtual void setQuadratureOrder(const libMesh::Order & quadrature_order) override;
  virtual void setQuadratureDimension(const unsigned int quadrature_dimension) override;
  virtual void initializeQuadratures() override;
  virtual bool quadraturesNotInitialized() override {return _surface_warehouse.quadraturesNotInitialized();}

protected:
  libMesh::Real calculateViewFactorKernel(const Segment<BaseKernel> & path);

  virtual bool isOccluded(const Segment<BaseKernel> & path) = 0;

  libMesh::Real calculateViewFactor(const SurfacePointer from_surf, const SurfacePointer to_surf);
  libMesh::Real getViewFactor(const SurfacePointer from_surf, const SurfacePointer to_surf);

protected:
  SymmetricDualMap<SurfacePointer, libMesh::Real> _view_factor_residuals;
  SurfaceWarehouse<BaseKernel> _surface_warehouse;

  SurfacePointer _from_surf;
  SurfacePointer _to_surf;

  typename QuadKernel::Vector_3 _from_normal;
  typename QuadKernel::Vector_3 _to_normal;

  // CGAL is smart enough to not apply any conversions if the source
  // and target kernels are the same. Thus, we can indiscriminately
  // apply these conversions without having to worry about
  // inefficiencies from a trivial conversion!
  CGAL::Cartesian_converter<BaseKernel, QuadKernel> _to_quad_kernel;
  CGAL::Cartesian_converter<BaseKernel, CollisionKernel> _to_collision_kernel;
};

template <class BaseKernel, class QuadKernel, class CollisionKernel>
SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::SurfaceConnector()
{
}

template <class BaseKernel, class QuadKernel, class CollisionKernel>
inline void
SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::setQuadratureType(const libMesh::QuadratureType & quadrature_type)
{
  _surface_warehouse.setQuadratureType(quadrature_type);
}

template <class BaseKernel, class QuadKernel, class CollisionKernel>
inline void
SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::setQuadratureOrder(const libMesh::Order & quadrature_order)
{
  _surface_warehouse.setQuadratureOrder(quadrature_order);
}

template <class BaseKernel, class QuadKernel, class CollisionKernel>
inline void
SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::setQuadratureDimension(const unsigned int quadrature_dimension)
{
  _surface_warehouse.setQuadratureDimension(quadrature_dimension);
}

template <class BaseKernel, class QuadKernel, class CollisionKernel>
inline void
SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::initializeQuadratures()
{
  _surface_warehouse.initializeQuadratures();
}

template <class BaseKernel, class QuadKernel, class CollisionKernel>
inline void
SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::buildSurface(const SurfaceID & surf_id, const libMesh::Node * a, const libMesh::Node * b, const libMesh::Node * c)
{
  _surface_warehouse.buildSurface(surf_id, a, b, c);
}

template <class BaseKernel, class QuadKernel, class CollisionKernel>
inline void
SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::buildSurface(const SurfaceID & surf_id, const libMesh::Node * a, const libMesh::Node * b, const libMesh::Node * c, const libMesh::Node * d)
{
  _surface_warehouse.buildSurface(surf_id, a, b, c, d);
}

// Assumes that the surface normals are the same over the entire surface
// (i.e. the surfaces are flat)
template <class BaseKernel, class QuadKernel, class CollisionKernel>
inline libMesh::Real
SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::calculateViewFactorKernel(
    const Segment<BaseKernel> & path)
{
  return rawViewFactorKernel<QuadKernel>(_to_quad_kernel(path.vector()), _from_normal, _to_normal);
}

template <class BaseKernel, class QuadKernel, class CollisionKernel>
inline libMesh::Real
SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::calculateViewFactor(const SurfacePointer from_surf, const SurfacePointer to_surf)
{
  bool not_connected = true;
  libMesh::Real residual = 0.0;

  typename BaseKernel::Vector_3 centroid_path(to_surf->getCentroid() - from_surf->getCentroid());

  bool source_facing_target = CGAL::angle(from_surf->getNormal(), centroid_path) == CGAL::ACUTE;
  bool target_facing_source = CGAL::angle(to_surf->getNormal(), centroid_path) == CGAL::OBTUSE;

  // only does calculation if the surfaces are facing eachother
  if (source_facing_target && target_facing_source)
  {
    _from_surf = from_surf;
    _to_surf = to_surf;

    _from_normal = normalize<QuadKernel>(_to_quad_kernel(_from_surf->getNormal()));
    _to_normal = normalize<QuadKernel>(_to_quad_kernel(_to_surf->getNormal()));

    for (auto & from_qp_pair : from_surf->getQpPairs())
    {
      const auto & from_qp(from_qp_pair.first);
      libMesh::Real from_JxW(from_qp_pair.second);

      libMesh::Real to_residual = 0.0;
      for (auto & to_qp_pair : to_surf->getQpPairs())
      {
        const auto & to_qp(to_qp_pair.first);
        libMesh::Real to_JxW(to_qp_pair.second);

        Segment<BaseKernel> path(from_qp, to_qp);
        bool occluded = isOccluded(path);
        not_connected = not_connected && occluded;

        if (!occluded)
        {
          libMesh::Real raw_kernel = calculateViewFactorKernel(path);
          to_residual += raw_kernel * to_JxW;
        }
      }
      residual += to_residual * from_JxW;
    }
  }

  return residual;
}

template <class BaseKernel, class QuadKernel, class CollisionKernel>
libMesh::Real
SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::getViewFactor(const SurfacePointer from_surf, const SurfacePointer to_surf)
{
  libMesh::Real residual = 0.0;

  if (from_surf != to_surf)
  {
    if (_view_factor_residuals.contains(from_surf, to_surf))
    {
      residual = _view_factor_residuals(from_surf, to_surf);
    }
    else
    {
      residual = calculateViewFactor(from_surf, to_surf);
      _view_factor_residuals(from_surf, to_surf) = residual;
      if (residual != 0.0)
      {
        from_surf->connect(*to_surf);
        to_surf->connect(*from_surf);
      }
    }
  }

  return residual / from_surf->getArea();
}

template <class BaseKernel, class QuadKernel, class CollisionKernel>
libMesh::Real
SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::getViewFactor(
  const SurfaceID & from_surf_id, const SurfaceID & to_surf_id)
{
  const SurfacePointer from_surf(_surface_warehouse.getSurface(from_surf_id));
  const SurfacePointer to_surf(_surface_warehouse.getSurface(to_surf_id));
  return getViewFactor(from_surf, to_surf);
}

template <class BaseKernel, class QuadKernel, class CollisionKernel>
libMesh::Real
SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::getArea(const SurfaceID & surf_id)
{
  const SurfacePointer surf(_surface_warehouse.getSurface(surf_id));
  return surf->getArea();
}

template <class BaseKernel, class QuadKernel, class CollisionKernel>
const std::vector<SurfaceID> &
SurfaceConnector<BaseKernel, QuadKernel, CollisionKernel>::getConnectedSurfaceIDs(const SurfaceID & surf_id)
{
  return _surface_warehouse.getSurface(surf_id)->getConnections();
}

}
