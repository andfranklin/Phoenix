#ifndef SURFACECONNECTORBASE_H
#define SURFACECONNECTORBASE_H

namespace libMesh
{
  class Node;
  enum QuadratureType : int;
  enum Order : int;
}

// Abstract base class for all SurfaceConnectors. Required so that
// all derived surface connectors can be dynamically created.

namespace Geom
{

class SurfaceConnectorBase
{
public:
  SurfaceConnectorBase() {};
  virtual ~SurfaceConnectorBase() {}

  virtual libMesh::Real getViewFactor(unsigned int from_surf_index, unsigned int to_surf_index) = 0;
  virtual libMesh::Real getArea(unsigned int surf_index) = 0;

  virtual void buildSurface(const libMesh::Node * a, const libMesh::Node * b, const libMesh::Node * c) = 0;
  virtual void buildSurface(const libMesh::Node * a, const libMesh::Node * b, const libMesh::Node * c, const libMesh::Node * d) = 0;

  virtual void setQuadratureType(const libMesh::QuadratureType & quadrature_type) = 0;
  virtual void setQuadratureOrder(const libMesh::Order & quadrature_order) = 0;
  virtual void setQuadratureDimension(const unsigned int quadrature_dimension) = 0;
  virtual void initializeQuadratures() = 0;
  virtual bool quadraturesNotInitialized() = 0;
};

}

#endif /* SURFACECONNECTORBASE_H */