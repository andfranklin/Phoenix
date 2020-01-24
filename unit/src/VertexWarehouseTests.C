#include "gtest/gtest.h"
#include "CGALTestingExtension.h"

#include "VertexWarehouse.h"

using namespace Geom;

template <class T>
class VertexWarehouseTests : public testing::Test
{
protected:
  virtual void SetUp()
  {
    _id_1 = 100;
    _example_node_1 = libMesh::Node::build(1, 2, 3, _id_1).release();

    _id_2 = 100;
    _example_node_2 = libMesh::Node::build(3, 2, 1, _id_2).release();
  }

  virtual void TearDown()
  {
    delete _example_node_1;
    delete _example_node_2;
  }

  libMesh::unique_id_type _id_1;
  libMesh::Node * _example_node_1;

  libMesh::unique_id_type _id_2;
  libMesh::Node * _example_node_2;

  VertexWarehouse<T> _warehouse;
};

TYPED_TEST_CASE_SETUP(VertexWarehouseTests);

TYPED_TEST(VertexWarehouseTests, getVertex)
{
  const std::shared_ptr<const Vertex<TypeParam>> test_vertex_1(
      this->_warehouse.getVertex(this->_example_node_1));
  EXPECT_THREE_TUPLE_EQ(test_vertex_1->point(), 1, 2, 3)

  const std::shared_ptr<const Vertex<TypeParam>> test_vertex_2(
      this->_warehouse.getVertex(this->_example_node_2));
  EXPECT_THREE_TUPLE_EQ(test_vertex_2->point(), 3, 2, 1)
}

TYPED_TEST(VertexWarehouseTests, memoization)
{
  this->_warehouse.getVertex(this->_example_node_1);
  this->_warehouse.getVertex(this->_example_node_2);
  this->_warehouse.getVertex(this->_example_node_2);
  this->_warehouse.getVertex(this->_example_node_1);

  ASSERT_EQ(this->_warehouse.size(), 2);
}