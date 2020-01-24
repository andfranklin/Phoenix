#include "gtest/gtest.h"
#include "CGALTestingExtension.h"

#include "Vertex.h"

using namespace Geom;

template <class T>
class VertexTests : public testing::Test
{
protected:
  virtual void SetUp()
  {
    _id = 100;
    _example_node = libMesh::Node::build(1, 2, 3, _id).release();
  }

  virtual void TearDown() { delete _example_node; }

  libMesh::unique_id_type _id;
  libMesh::Node * _example_node;
};

TYPED_TEST_CASE_SETUP(VertexTests);

TYPED_TEST(VertexTests, pointRepr)
{
  Vertex<TypeParam> test_vertex(this->_example_node);
  EXPECT_THREE_TUPLE_EQ(test_vertex.point(), 1, 2, 3)
}

TYPED_TEST(VertexTests, locusRepr)
{
  Vertex<TypeParam> test_vertex(this->_example_node);
  EXPECT_THREE_TUPLE_EQ(test_vertex.locus(), 1, 2, 3)
}

TYPED_TEST(VertexTests, bbox)
{
  Vertex<TypeParam> test_vertex(this->_example_node);
  auto bbox = test_vertex.bbox();
  EXPECT_BBOX_EQ(bbox, 1, 1, 2, 2, 3, 3)
}

TYPED_TEST(VertexTests, projectOntoPlane)
{
  typename TypeParam::Plane_3 plane({0, 0, 0}, {1, 0, 0}, {0, 1, 0});
  Vertex<TypeParam> test_vertex(this->_example_node);
  Vertex<TypeParam> projection(test_vertex.projectOnto(plane));

  EXPECT_THREE_TUPLE_EQ(projection.point(), 1, 2, 0)
}

TYPED_TEST(VertexTests, projectOntoTriangle)
{
  typename TypeParam::Triangle_3 triangle({0, 0, 0}, {1, 0, 0}, {0, 1, 0});
  Vertex<TypeParam> test_vertex(this->_example_node);
  Vertex<TypeParam> projection(test_vertex.projectOnto(triangle));

  EXPECT_THREE_TUPLE_EQ(projection.point(), 1, 2, 0)
}

TYPED_TEST(VertexTests, buildTriangle)
{
  using vertex_t = const Vertex<TypeParam>;
  using vertex_ptr_t = std::shared_ptr<vertex_t>;

  typename TypeParam::Point_3 point_a(0, 0, 0);
  vertex_ptr_t a(std::make_shared<vertex_t>(point_a));

  typename TypeParam::Point_3 point_b(2, 0, 0);
  vertex_ptr_t b(std::make_shared<vertex_t>(point_b));

  typename TypeParam::Point_3 point_c(0, 3, 0);
  vertex_ptr_t c(std::make_shared<vertex_t>(point_c));

  typename TypeParam::Triangle_3 triangle(buildTriangle<TypeParam>(a, b, c));

  EXPECT_THREE_TUPLE_EQ(getNormal<TypeParam>(triangle), 0, 0, 6)
  ASSERT_FT_EQ(getArea<TypeParam>(triangle), 3);
}