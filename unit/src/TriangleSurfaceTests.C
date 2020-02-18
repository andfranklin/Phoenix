#include "gtest/gtest.h"
#include "CGALTestingExtension.h"
#include "libmesh/enum_quadrature_type.h"

#include "TriangleSurface.h"
#include "Segment.h"

using namespace Geom;

// Makes `getSurfaceLocation` and `getAbsJacobian` publicly available
template <class T>
class TestableTriangleSurface : public TriangleSurface<T>
{
public:
  using TriangleSurface<T>::TriangleSurface;
  using Surface<T>::getSurfaceLocation;
  using Surface<T>::getAbsJacobian;
};

template <class T>
class TriangleSurfaceTests : public testing::Test
{
protected:
  virtual void SetUp()
  {
    libMesh::Node * node_1 = libMesh::Node::build(0.0, 0.0, 0.0, 1).release();
    libMesh::Node * node_2 = libMesh::Node::build(2.0, 0.0, 0.0, 2).release();
    libMesh::Node * node_3 = libMesh::Node::build(0.0, 2.0, 0.0, 3).release();

    VertexWarehouse<T> vertex_warehouse;

    const std::shared_ptr<const Vertex<T>> vertex_1(vertex_warehouse.getVertex(node_1));
    const std::shared_ptr<const Vertex<T>> vertex_2(vertex_warehouse.getVertex(node_2));
    const std::shared_ptr<const Vertex<T>> vertex_3(vertex_warehouse.getVertex(node_3));

    delete node_1;
    delete node_2;
    delete node_3;

    _triangle = new TestableTriangleSurface<T>(vertex_1, vertex_2, vertex_3);

    std::unique_ptr<libMesh::QBase> quadrature =
        libMesh::QBase::build(libMesh::QGAUSS, 2, libMesh::SECOND);
    quadrature->init(libMesh::TRI3, 0); // 2D, 2nd-order GQ for a TRI3 element

    _triangle->init(*quadrature);
  }

  virtual void TearDown() { delete _triangle; }

  TestableTriangleSurface<T> * _triangle;
};

TYPED_TEST_CASE_SETUP(TriangleSurfaceTests);

TYPED_TEST(TriangleSurfaceTests, getNormal)
{
  const typename TypeParam::Vector_3 & normal(this->_triangle->getNormal());
  EXPECT_THREE_TUPLE_EQ(normal, 0.0, 0.0, 2.0);
}

TYPED_TEST(TriangleSurfaceTests, getArea) { EXPECT_DOUBLE_EQ(this->_triangle->getArea(), 2.0); }

TYPED_TEST(TriangleSurfaceTests, getSurfaceLocation)
{
  const typename TypeParam::Point_3 & surface_point(this->_triangle->getSurfaceLocation(0.5, 0.5));
  EXPECT_THREE_TUPLE_EQ(surface_point, 1.0, 1.0, 0.0);
}

TYPED_TEST(TriangleSurfaceTests, getAbsJacobian)
{
  EXPECT_DOUBLE_EQ(this->_triangle->getAbsJacobian(0.5, 0.5), 4.0);
}

TYPED_TEST(TriangleSurfaceTests, HeadOnIntersection)
{
  Segment<TypeParam> seg({0.25, 0.25, 1.0}, {0.25, 0.25, -1.0});
  EXPECT_TRUE(this->_triangle->definitiveIntersection(seg));
  EXPECT_TRUE(seg.isHeadOnWith(this->_triangle));
}

TYPED_TEST(TriangleSurfaceTests, NoneHeadOnIntersection)
{
  Segment<TypeParam> seg({0.25, 0.25, -1.0}, {0.25, 0.25, 1.0});
  EXPECT_TRUE(this->_triangle->definitiveIntersection(seg));
  EXPECT_FALSE(seg.isHeadOnWith(this->_triangle));
}

TYPED_TEST(TriangleSurfaceTests, HeadOnNoneIntersection)
{
  Segment<TypeParam> seg({2.25, 2.25, 1.0}, {2.25, 2.25, -1.0});
  EXPECT_FALSE(this->_triangle->definitiveIntersection(seg));
  EXPECT_TRUE(seg.isHeadOnWith(this->_triangle));
}

TYPED_TEST(TriangleSurfaceTests, NoneHeadOnNoneIntersection)
{
  Segment<TypeParam> seg({2.25, 2.25, -1.0}, {2.25, 2.25, 1.0});
  EXPECT_FALSE(this->_triangle->definitiveIntersection(seg));
  EXPECT_FALSE(seg.isHeadOnWith(this->_triangle));
}

TYPED_TEST(TriangleSurfaceTests, bbox)
{
  auto bbox = this->_triangle->bbox();
  EXPECT_BBOX_EQ(bbox, 0, 2, 0, 2, 0, 0)
}