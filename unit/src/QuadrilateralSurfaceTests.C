#include "gtest/gtest.h"
#include "CGALTestingExtension.h"
#include "libmesh/enum_quadrature_type.h"

#include "QuadrilateralSurface.h"
#include "Segment.h"

using namespace Geom;

// Makes `getSurfaceLocation` and `getAbsJacobian` publicly available
template <class T>
class TestableQuadrilateralSurface : public QuadrilateralSurface<T>
{
public:
  using QuadrilateralSurface<T>::QuadrilateralSurface;
  using Surface<T>::getSurfaceLocation;
  using Surface<T>::getAbsJacobian;
};

template <class T>
class QuadrilateralSurfaceTests : public testing::Test
{
protected:
  virtual void SetUp()
  {
    libMesh::Node * node_1 = libMesh::Node::build(0.0, 0.0, 0.0, 1).release();
    libMesh::Node * node_2 = libMesh::Node::build(2.0, 0.0, 0.0, 2).release();
    libMesh::Node * node_3 = libMesh::Node::build(2.0, 5.0, 0.0, 3).release();
    libMesh::Node * node_4 = libMesh::Node::build(0.0, 5.0, 0.0, 4).release();

    VertexWarehouse<T> vertex_warehouse;

    const std::shared_ptr<const Vertex<T>> vertex_1(vertex_warehouse.getVertex(node_1));
    const std::shared_ptr<const Vertex<T>> vertex_2(vertex_warehouse.getVertex(node_2));
    const std::shared_ptr<const Vertex<T>> vertex_3(vertex_warehouse.getVertex(node_3));
    const std::shared_ptr<const Vertex<T>> vertex_4(vertex_warehouse.getVertex(node_4));

    delete node_1;
    delete node_2;
    delete node_3;
    delete node_4;

    _quadrilateral = new TestableQuadrilateralSurface<T>(vertex_1, vertex_2, vertex_3, vertex_4);

    std::unique_ptr<libMesh::QBase> quadrature =
        libMesh::QBase::build(libMesh::QGAUSS, 2, libMesh::SECOND);
    quadrature->init(libMesh::QUAD4, 0); // 2D, 2nd-order GQ for a QUAD4 element

    _quadrilateral->init(*quadrature);
  }

  virtual void TearDown() { delete _quadrilateral; }

  TestableQuadrilateralSurface<T> * _quadrilateral;
};

TYPED_TEST_CASE_SETUP(QuadrilateralSurfaceTests);

TYPED_TEST(QuadrilateralSurfaceTests, getNormal)
{
  const typename TypeParam::Vector_3 & normal(this->_quadrilateral->getNormal());
  EXPECT_THREE_TUPLE_EQ(normal, 0.0, 0.0, 10.0);
}

TYPED_TEST(QuadrilateralSurfaceTests, getArea)
{
  EXPECT_DOUBLE_EQ(this->_quadrilateral->getArea(), 10.0);
}

TYPED_TEST(QuadrilateralSurfaceTests, getSurfaceLocation)
{
  const typename TypeParam::Point_3 & surface_point(
      this->_quadrilateral->getSurfaceLocation(0.0, 0.0));
  EXPECT_THREE_TUPLE_EQ(surface_point, 1.0, 2.5, 0.0);
}

TYPED_TEST(QuadrilateralSurfaceTests, getAbsJacobian)
{
  EXPECT_DOUBLE_EQ(this->_quadrilateral->getAbsJacobian(0.5, 0.5), 2.5);
}

TYPED_TEST(QuadrilateralSurfaceTests, HeadOnIntersection)
{
  Segment<TypeParam> seg({0.25, 0.25, 1.0}, {0.25, 0.25, -1.0});
  EXPECT_TRUE(this->_quadrilateral->definitiveIntersection(seg));
  EXPECT_TRUE(seg.isHeadOnWith(this->_quadrilateral));
}

TYPED_TEST(QuadrilateralSurfaceTests, NoneHeadOnIntersection)
{
  Segment<TypeParam> seg({0.25, 0.25, -1.0}, {0.25, 0.25, 1.0});
  EXPECT_TRUE(this->_quadrilateral->definitiveIntersection(seg));
  EXPECT_FALSE(seg.isHeadOnWith(this->_quadrilateral));
}

TYPED_TEST(QuadrilateralSurfaceTests, HeadOnNoneIntersection)
{
  Segment<TypeParam> seg({2.25, 2.25, 1.0}, {2.25, 2.25, -1.0});
  EXPECT_FALSE(this->_quadrilateral->definitiveIntersection(seg));
  EXPECT_TRUE(seg.isHeadOnWith(this->_quadrilateral));
}

TYPED_TEST(QuadrilateralSurfaceTests, NoneHeadOnNoneIntersection)
{
  Segment<TypeParam> seg({2.25, 2.25, -1.0}, {2.25, 2.25, 1.0});
  EXPECT_FALSE(this->_quadrilateral->definitiveIntersection(seg));
  EXPECT_FALSE(seg.isHeadOnWith(this->_quadrilateral));
}

TYPED_TEST(QuadrilateralSurfaceTests, bbox)
{
  auto bbox = this->_quadrilateral->bbox();
  EXPECT_BBOX_EQ(bbox, 0, 2, 0, 5, 0, 0)
}