#include "gtest/gtest.h"
#include "CGALTestingExtension.h"
#include "libmesh/enum_quadrature_type.h"

#include "SurfaceWarehouse.h"

using namespace Geom;

template <class T>
class SurfaceWarehouseTests : public testing::Test
{
protected:
  virtual void SetUp()
  {
    _quadrature_type = libMesh::QGAUSS;
    _quadrature_order = libMesh::SECOND;

    _surface_warehouse = new SurfaceWarehouse<T>();
    _surface_warehouse->setQuadratureType(_quadrature_type);
    _surface_warehouse->setQuadratureOrder(_quadrature_order);
    _surface_warehouse->setQuadratureDimension(2);
    _surface_warehouse->initializeQuadratures();
  }

  virtual void TearDown() { delete _surface_warehouse; }

  libMesh::QuadratureType _quadrature_type;
  libMesh::Order _quadrature_order;
  SurfaceWarehouse<T> * _surface_warehouse;
};

TYPED_TEST_CASE_SETUP(SurfaceWarehouseTests);

TYPED_TEST(SurfaceWarehouseTests, buildTriangleSurface)
{
  // This test builds the same triangle surface as in
  // TriangleSurfaceTests and verifies that it has all of the same
  // properties
  libMesh::Node * node_1 = libMesh::Node::build(0.0, 0.0, 0.0, 1).release();
  libMesh::Node * node_2 = libMesh::Node::build(2.0, 0.0, 0.0, 2).release();
  libMesh::Node * node_3 = libMesh::Node::build(0.0, 2.0, 0.0, 3).release();

  SurfaceID surf_id = {0, 0};
  this->_surface_warehouse->buildSurface(surf_id, node_1, node_2, node_3);
  std::shared_ptr<SurfaceBase<TypeParam>> triangle = this->_surface_warehouse->getSurfaces()[0];

  delete node_1;
  delete node_2;
  delete node_3;

  // verify the triangle's properties
  EXPECT_DOUBLE_EQ(triangle->getArea(), 2.0);
  EXPECT_THREE_TUPLE_EQ(triangle->getNormal(), 0.0, 0.0, 2.0);

  const auto & qps(triangle->getEmbeddedQps());
  EXPECT_THREE_TUPLE_EQ(qps[0], (4.0 / 3.0), (1.0 / 3.0), 0.0);
  EXPECT_THREE_TUPLE_EQ(qps[1], (1.0 / 3.0), (4.0 / 3.0), 0.0);
  EXPECT_THREE_TUPLE_EQ(qps[2], (1.0 / 3.0), (1.0 / 3.0), 0.0);

  const auto & jxws(triangle->getJxWs());
  EXPECT_DOUBLE_EQ(jxws[0], (2.0 / 3.0));
  EXPECT_DOUBLE_EQ(jxws[1], (2.0 / 3.0));
  EXPECT_DOUBLE_EQ(jxws[2], (2.0 / 3.0));

  EXPECT_BBOX_EQ(triangle->bbox(), 0, 2, 0, 2, 0, 0)
}

TYPED_TEST(SurfaceWarehouseTests, buildQuadrilateralSurface)
{
  constexpr double i_sqrt_3 = 0.57735026918962576450914878050196;
  constexpr double one_m_i_sqrt_3 = 1.0 - i_sqrt_3;
  constexpr double one_p_i_sqrt_3 = 1.0 + i_sqrt_3;

  // This test builds the same quadrilateral surface as in
  // QuadrilateralSurfaceTests and verifies that it has all of the same
  // properties
  libMesh::Node * node_1 = libMesh::Node::build(0.0, 0.0, 0.0, 1).release();
  libMesh::Node * node_2 = libMesh::Node::build(2.0, 0.0, 0.0, 2).release();
  libMesh::Node * node_3 = libMesh::Node::build(2.0, 5.0, 0.0, 3).release();
  libMesh::Node * node_4 = libMesh::Node::build(0.0, 5.0, 0.0, 4).release();

  SurfaceID surf_id = {0, 0};
  this->_surface_warehouse->buildSurface(surf_id, node_1, node_2, node_3, node_4);
  std::shared_ptr<SurfaceBase<TypeParam>> quadrilateral =
      this->_surface_warehouse->getSurfaces()[0];

  delete node_1;
  delete node_2;
  delete node_3;
  delete node_4;

  // verify the quadrilateral properties
  EXPECT_DOUBLE_EQ(quadrilateral->getArea(), 10.0);
  EXPECT_THREE_TUPLE_EQ(quadrilateral->getNormal(), 0.0, 0.0, 10.0);

  const auto & qps(quadrilateral->getEmbeddedQps());
  EXPECT_THREE_TUPLE_EQ(qps[0], one_m_i_sqrt_3, (2.5 * one_m_i_sqrt_3), 0.0);
  EXPECT_THREE_TUPLE_EQ(qps[1], one_p_i_sqrt_3, (2.5 * one_m_i_sqrt_3), 0.0);
  EXPECT_THREE_TUPLE_EQ(qps[2], one_m_i_sqrt_3, (2.5 * one_p_i_sqrt_3), 0.0);
  EXPECT_THREE_TUPLE_EQ(qps[3], one_p_i_sqrt_3, (2.5 * one_p_i_sqrt_3), 0.0);

  const auto & jxws(quadrilateral->getJxWs());
  EXPECT_DOUBLE_EQ(jxws[0], 2.5);
  EXPECT_DOUBLE_EQ(jxws[1], 2.5);
  EXPECT_DOUBLE_EQ(jxws[2], 2.5);
  EXPECT_DOUBLE_EQ(jxws[3], 2.5);

  EXPECT_BBOX_EQ(quadrilateral->bbox(), 0, 2, 0, 5, 0, 0)
}
