#ifndef SURFACEWAREHOUSE_H
#define SURFACEWAREHOUSE_H

#include "TriangleSurface.h"
#include "QuadrilateralSurface.h"
#include "libmesh/quadrature.h"

// Builds and stores surfaces provided appropriately ordered nodes.
// Also responsible for initializing quadratures because surfaces
// refer to them.

namespace Geom
{

template <class T>
class SurfaceWarehouse
{
public:
  SurfaceWarehouse(unsigned int n_surfaces = 0);
  virtual ~SurfaceWarehouse() {}

  void setQuadratureType(const libMesh::QuadratureType & quadrature_type);
  void setQuadratureOrder(const libMesh::Order & quadrature_order);
  void setQuadratureDimension(const unsigned int quadrature_dimension);
  void initializeQuadratures();
  bool quadraturesNotInitialized() {return !_quadratures_initialized;}

  void buildSurface(const libMesh::Node * a, const libMesh::Node * b, const libMesh::Node * c);

  void buildSurface(const libMesh::Node * a,
                    const libMesh::Node * b,
                    const libMesh::Node * c,
                    const libMesh::Node * d);

  std::shared_ptr<SurfaceBase<T>> getSurface(unsigned int surf_index) const { return _surfaces[surf_index]; }
  const std::vector<std::shared_ptr<SurfaceBase<T>>> & getSurfaces() const { return _surfaces; }
  inline unsigned int getNumberOfSurfaces() const { return _surfaces.size(); }

protected:
  bool _quadrature_type_set;
  libMesh::QuadratureType _quadrature_type;

  bool _quadrature_order_set;
  libMesh::Order _quadrature_order;

  bool _quadrature_dimension_set;
  unsigned int _quadrature_dimension;

  bool _quadratures_initialized;

  std::shared_ptr<libMesh::QBase> _triangle_quadrature;
  std::shared_ptr<libMesh::QBase> _quadrilateral_quadrature;

  VertexWarehouse<T> _vertex_warehouse;
  std::vector<std::shared_ptr<SurfaceBase<T>>> _surfaces;
};

template <class T>
SurfaceWarehouse<T>::SurfaceWarehouse(unsigned int n_surfaces)
  : _quadrature_type_set(false), _quadrature_order_set(false), _quadratures_initialized(false)
{
  _surfaces.reserve(n_surfaces);
}

template <class T>
void
SurfaceWarehouse<T>::setQuadratureType(const libMesh::QuadratureType & quadrature_type)
{
  if (!_quadrature_type_set)
  {
    _quadrature_type = quadrature_type;
    _quadrature_type_set = true;
  }
}

template <class T>
void
SurfaceWarehouse<T>::setQuadratureOrder(const libMesh::Order & quadrature_order)
{
  if (!_quadrature_order_set)
  {
    _quadrature_order = quadrature_order;
    _quadrature_order_set = true;
  }
}

template <class T>
void
SurfaceWarehouse<T>::setQuadratureDimension(const unsigned int quadrature_order)
{
  if (!_quadrature_dimension_set)
  {
    _quadrature_dimension = quadrature_order;
    _quadrature_dimension_set = true;
  }
}

template <class T>
void
SurfaceWarehouse<T>::initializeQuadratures()
{
  if (_quadrature_type_set && _quadrature_order_set && _quadrature_dimension_set)
  {
    unsigned int p_level = 0;

    _triangle_quadrature = libMesh::QBase::build(_quadrature_type, _quadrature_dimension, _quadrature_order);
    _triangle_quadrature->init(libMesh::TRI3, p_level);

    _quadrilateral_quadrature = libMesh::QBase::build(_quadrature_type, _quadrature_dimension, _quadrature_order);
    _quadrilateral_quadrature->init(libMesh::QUAD4, p_level);

    _quadratures_initialized = true;
  }
}

template <class T>
void
SurfaceWarehouse<T>::buildSurface(const libMesh::Node * a,
                                  const libMesh::Node * b,
                                  const libMesh::Node * c)
{
  using surface_t = TriangleSurface<T>;

  const std::shared_ptr<const Vertex<T>> vertex_a(_vertex_warehouse.getVertex(a));
  const std::shared_ptr<const Vertex<T>> vertex_b(_vertex_warehouse.getVertex(b));
  const std::shared_ptr<const Vertex<T>> vertex_c(_vertex_warehouse.getVertex(c));

  std::shared_ptr<surface_t> triangle = std::make_shared<surface_t>(vertex_a, vertex_b, vertex_c);
  triangle->init(*_triangle_quadrature);
  _surfaces.push_back(triangle);
}

template <class T>
void
SurfaceWarehouse<T>::buildSurface(const libMesh::Node * a,
                                  const libMesh::Node * b,
                                  const libMesh::Node * c,
                                  const libMesh::Node * d)
{
  using surface_t = QuadrilateralSurface<T>;

  const std::shared_ptr<const Vertex<T>> vertex_a(_vertex_warehouse.getVertex(a));
  const std::shared_ptr<const Vertex<T>> vertex_b(_vertex_warehouse.getVertex(b));
  const std::shared_ptr<const Vertex<T>> vertex_c(_vertex_warehouse.getVertex(c));
  const std::shared_ptr<const Vertex<T>> vertex_d(_vertex_warehouse.getVertex(d));

  std::shared_ptr<surface_t> quadrilateral =
      std::make_shared<surface_t>(vertex_a, vertex_b, vertex_c, vertex_d);
  quadrilateral->init(*_quadrilateral_quadrature);

  _surfaces.push_back(quadrilateral);
}

}

#endif /* SURFACEWAREHOUSE_H */