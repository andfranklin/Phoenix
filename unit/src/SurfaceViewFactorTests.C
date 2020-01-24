#include "gtest/gtest.h"
#include "CGALTestingExtension.h"
#include "libmesh/enum_quadrature_type.h"

#include "NoOcclusionSurfaceConnector.h"
#include "BruteForceWithBBCSurfaceConnector.h"
#include "BruteForceWithoutBBCSurfaceConnector.h"

using namespace Geom;

template <class SurfaceConnector, class BaseKernel>
struct TypeDefinition
{
  using MySurfaceConnector = SurfaceConnector;
  using MyBaseKernel = BaseKernel;
};

template <class T>
class SurfaceViewFactorTests : public testing::Test
{
protected:
  using SurfaceConnector = typename T::MySurfaceConnector;
  using BaseKernel = typename T::MyBaseKernel;

  virtual void SetUp()
  {
    _coarse_surface_connector = new SurfaceConnector();
    _fine_surface_connector = new SurfaceConnector();

    _n_surfaces = 6;
    _quadrature_type = libMesh::QGAUSS;
    _coarse_quad_order = libMesh::SECOND;
    _fine_quad_order = libMesh::FIFTEENTH;
    // _fine_quad_order = libMesh::THIRTIETH;

    _coarse_surface_connector->setQuadratureType(_quadrature_type);
    _coarse_surface_connector->setQuadratureOrder(_coarse_quad_order);
    _coarse_surface_connector->setQuadratureDimension(2);
    _coarse_surface_connector->initializeQuadratures();

    _fine_surface_connector->setQuadratureType(_quadrature_type);
    _fine_surface_connector->setQuadratureOrder(_fine_quad_order);
    _fine_surface_connector->setQuadratureDimension(2);
    _fine_surface_connector->initializeQuadratures();

    libMesh::Node * node_1(nullptr);
    libMesh::Node * node_2(nullptr);
    libMesh::Node * node_3(nullptr);
    libMesh::Node * node_4(nullptr);
    libMesh::Node * node_5(nullptr);
    libMesh::Node * node_6(nullptr);
    libMesh::Node * node_7(nullptr);
    libMesh::Node * node_8(nullptr);
    libMesh::Node * node_9(nullptr);
    libMesh::Node * node_10(nullptr);
    libMesh::Node * node_11(nullptr);
    libMesh::Node * node_12(nullptr);
    libMesh::Node * node_13(nullptr);
    libMesh::Node * node_14(nullptr);
    libMesh::Node * node_15(nullptr);
    libMesh::Node * node_16(nullptr);
    libMesh::Node * node_17(nullptr);
    libMesh::Node * node_18(nullptr);
    libMesh::Node * node_19(nullptr);
    libMesh::Node * node_20(nullptr);
    libMesh::Node * node_21(nullptr);

    // Triangle 1 (surface 0)
    node_1 = libMesh::Node::build(0.0, 0.0, 0.0, 1).release();
    node_2 = libMesh::Node::build(1.0, 0.0, 0.0, 2).release();
    node_3 = libMesh::Node::build(0.0, 1.0, 0.0, 3).release();
    _coarse_surface_connector->buildSurface(node_1, node_2, node_3);
    _fine_surface_connector->buildSurface(node_1, node_2, node_3);

    // Triangle 2 (surface 1)
    node_4 = libMesh::Node::build(0.0, 0.0, 1.0, 4).release();
    node_5 = libMesh::Node::build(0.0, 1.0, 1.0, 5).release();
    node_6 = libMesh::Node::build(1.0, 0.0, 1.0, 6).release();
    _coarse_surface_connector->buildSurface(node_4, node_5, node_6);
    _fine_surface_connector->buildSurface(node_4, node_5, node_6);

    // Triangle 3 (surface 2)
    // (facing in the same direction as Triangle 1)
    // Essentially Triangle 2 with its orientation flipped and
    // shifted in the positive x-direction 2.5 units.
    node_7 = libMesh::Node::build(2.5, 0.0, 1.0, 7).release();
    node_8 = libMesh::Node::build(3.5, 0.0, 1.0, 8).release();
    node_9 = libMesh::Node::build(2.5, 1.0, 1.0, 9).release();
    _coarse_surface_connector->buildSurface(node_7, node_8, node_9);
    _fine_surface_connector->buildSurface(node_7, node_8, node_9);

    // The squares have a similar setup to the triangles except they
    // are shifted in the positive y-direction 5 units. They have a
    // larger area of 2 units

    // Square 1 (surface 3)
    node_10 = libMesh::Node::build(0.0, 5.0, 0.0, 10).release();
    node_11 = libMesh::Node::build(2.0, 5.0, 0.0, 11).release();
    node_12 = libMesh::Node::build(2.0, 7.0, 0.0, 12).release();
    node_13 = libMesh::Node::build(0.0, 7.0, 0.0, 13).release();
    _coarse_surface_connector->buildSurface(node_10, node_11, node_12, node_13);
    _fine_surface_connector->buildSurface(node_10, node_11, node_12, node_13);

    // Square 2 (surface 4)
    node_14 = libMesh::Node::build(0.0, 5.0, 1.0, 14).release();
    node_15 = libMesh::Node::build(0.0, 7.0, 1.0, 15).release();
    node_16 = libMesh::Node::build(2.0, 7.0, 1.0, 16).release();
    node_17 = libMesh::Node::build(2.0, 5.0, 1.0, 17).release();
    _coarse_surface_connector->buildSurface(node_14, node_15, node_16, node_17);
    _fine_surface_connector->buildSurface(node_14, node_15, node_16, node_17);

    // Square 3 (surface 5)
    // (facing in the same direction as Square 1)
    // Essentially Square 2 with its orientation flipped and
    // shifted in the positive x-direction 2.5 units.
    node_18 = libMesh::Node::build(2.5, 5.0, 1.0, 18).release();
    node_19 = libMesh::Node::build(4.5, 5.0, 1.0, 19).release();
    node_20 = libMesh::Node::build(4.5, 7.0, 1.0, 20).release();
    node_21 = libMesh::Node::build(2.5, 7.0, 1.0, 21).release();
    _coarse_surface_connector->buildSurface(node_18, node_19, node_20, node_21);
    _fine_surface_connector->buildSurface(node_18, node_19, node_20, node_21);

    delete node_1;
    delete node_2;
    delete node_3;
    delete node_4;
    delete node_5;
    delete node_6;
    delete node_7;
    delete node_8;
    delete node_9;
    delete node_10;
    delete node_11;
    delete node_12;
    delete node_13;
    delete node_14;
    delete node_15;
    delete node_16;
    delete node_17;
    delete node_18;
    delete node_19;
    delete node_20;
    delete node_21;
  }

  virtual void TearDown()
  {
    delete _coarse_surface_connector;
    delete _fine_surface_connector;
  }

  libMesh::Real _n_surfaces;
  libMesh::QuadratureType _quadrature_type;
  libMesh::Order _coarse_quad_order;
  libMesh::Order _fine_quad_order;

  SurfaceConnectorBase * _coarse_surface_connector;
  SurfaceConnectorBase * _fine_surface_connector;
};

#define TYPE_DEFINITION(surf_connector, base_kernel, quad_kernel, collision_kernel)                \
  TypeDefinition<surf_connector<base_kernel, quad_kernel, collision_kernel>, base_kernel>

#define COLLISION_COMBO(surf_connector, base_kernel, quad_kernel)                                  \
  TYPE_DEFINITION(surf_connector, base_kernel, quad_kernel, Apprx)                                 \
  , TYPE_DEFINITION(surf_connector, base_kernel, quad_kernel, Exact)

#define QUAD_COMBO(surf_connector, base_kernel)                                                    \
  COLLISION_COMBO(surf_connector, base_kernel, Apprx)                                              \
  , COLLISION_COMBO(surf_connector, base_kernel, Exact)

#define SURFACE_CONNECTOR_COMBO(surf_connector)                                                    \
  QUAD_COMBO(surf_connector, Apprx), QUAD_COMBO(surf_connector, Exact)

typedef testing::Types<SURFACE_CONNECTOR_COMBO(NoOcclusionSurfaceConnector),
                       SURFACE_CONNECTOR_COMBO(BruteForceWithBBCSurfaceConnector),
                       SURFACE_CONNECTOR_COMBO(BruteForceWithoutBBCSurfaceConnector)>
    MyTypes;

TYPED_TEST_CASE(SurfaceViewFactorTests, MyTypes);

TYPED_TEST(SurfaceViewFactorTests, TriTriCoarse)
{
  // solution generated from external python script using adaptive
  // quadrature to achieve a high precision solution
  constexpr double solution = 0.115049228149610507920819202354;
  constexpr double abs_tol = 5.0e-3;

  EXPECT_NEAR(
      this->_coarse_surface_connector->getViewFactor(0, 1),
      solution,
      abs_tol);
  EXPECT_NEAR(
      this->_coarse_surface_connector->getViewFactor(1, 0),
      solution,
      abs_tol);
}

TYPED_TEST(SurfaceViewFactorTests, TriTriFine)
{
  // solution generated from external python script using adaptive
  // quadrature to achieve a high precision solution
  constexpr double solution = 0.115049228149610507920819202354;
  constexpr double abs_tol = 1.0e-11;

  EXPECT_NEAR(this->_fine_surface_connector->getViewFactor(0, 1),
              solution,
              abs_tol);
  EXPECT_NEAR(this->_fine_surface_connector->getViewFactor(1, 0),
              solution,
              abs_tol);
}

TYPED_TEST(SurfaceViewFactorTests, TriTriDegenerate)
{
  constexpr double solution = 0.0;

  EXPECT_DOUBLE_EQ(
      this->_coarse_surface_connector->getViewFactor(0, 2),
      solution);
  EXPECT_DOUBLE_EQ(
      this->_coarse_surface_connector->getViewFactor(2, 0),
      solution);
}

TYPED_TEST(SurfaceViewFactorTests, QuadQuadCoarse)
{
  constexpr double solution = 0.415253283577146636851296079839; // analytic solution
  constexpr double abs_tol = 5.0e-2;

  EXPECT_NEAR(
      this->_coarse_surface_connector->getViewFactor(3, 4),
      solution,
      abs_tol);
  EXPECT_NEAR(
      this->_coarse_surface_connector->getViewFactor(4, 3),
      solution,
      abs_tol);
}

TYPED_TEST(SurfaceViewFactorTests, QuadQuadFine)
{
  constexpr double solution = 0.415253283577146636851296079839; // analytic solution
  constexpr double abs_tol = 1.0e-6;

  EXPECT_NEAR(this->_fine_surface_connector->getViewFactor(3, 4),
              solution,
              abs_tol);
  EXPECT_NEAR(this->_fine_surface_connector->getViewFactor(4, 3),
              solution,
              abs_tol);
}

TYPED_TEST(SurfaceViewFactorTests, QuadQuadDegenerate)
{
  constexpr double solution = 0.0;

  EXPECT_DOUBLE_EQ(
      this->_coarse_surface_connector->getViewFactor(3, 5),
      solution);
  EXPECT_DOUBLE_EQ(
      this->_coarse_surface_connector->getViewFactor(5, 3),
      solution);
}

TYPED_TEST(SurfaceViewFactorTests, TriQuadCoarse)
{
  // solution found by using external python script that uses adaptive
  // quadrature to achieve a high precision solution
  constexpr double solution_0_to_4 = 0.001234381940290304492668926883;
  constexpr double solution_4_to_0 = (solution_0_to_4 * 0.5) / 4.0;
  constexpr double abs_tol = 1.0e-5;

  EXPECT_NEAR(
      this->_coarse_surface_connector->getViewFactor(0, 4),
      solution_0_to_4,
      abs_tol);
  EXPECT_NEAR(
      this->_coarse_surface_connector->getViewFactor(4, 0),
      solution_4_to_0,
      abs_tol);
}

TYPED_TEST(SurfaceViewFactorTests, TriQuadFine)
{
  // solution found by using external python script that uses adaptive
  // quadrature to achieve a high precision solution
  constexpr double solution_0_to_4 = 0.001234381940290304492668926883;
  constexpr double solution_4_to_0 = (solution_0_to_4 * 0.5) / 4.0;
  constexpr double abs_tol = 1.0e-15;

  EXPECT_NEAR(this->_fine_surface_connector->getViewFactor(0, 4),
              solution_0_to_4,
              abs_tol);
  EXPECT_NEAR(this->_fine_surface_connector->getViewFactor(4, 0),
              solution_4_to_0,
              abs_tol);
}