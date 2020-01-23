#include "gtest/gtest.h"
#include "CGALTestingExtension.h"

#include "GeometryUtils.h"

using namespace Geom;

template <class T>
class GeomUtilsTests : public testing::Test
{
protected:
  virtual void SetUp()
  {
    _test_vect = {3.0, 4.0, 12.0}; // length = 13, lenght^2 = 169
    _test_triangle = {{0, 0, 0}, {2, 0, 0}, {0, 3, 0}}; // normal = (0, 0, 6), area = 3
  }

  typename T::Vector_3 _test_vect;
  typename T::Triangle_3 _test_triangle;
};

TYPED_TEST_CASE_SETUP(GeomUtilsTests);

TYPED_TEST(GeomUtilsTests, toReal)
{
  typename TypeParam::FT squared_length(this->_test_vect.squared_length());
  ASSERT_DOUBLE_EQ(toReal<typename TypeParam::FT>(squared_length), 169.0);
}

TYPED_TEST(GeomUtilsTests, apprxSqrt)
{
  typename TypeParam::FT squared_length(this->_test_vect.squared_length());
  ASSERT_DOUBLE_EQ(apprxSqrt<TypeParam>(squared_length), 13.0);
}

TYPED_TEST(GeomUtilsTests, isHeadOn)
{
  using Vector_3 = typename TypeParam::Vector_3;
  const Vector_3 & test_vect(this->_test_vect);

  EXPECT_FALSE(isHeadOn<TypeParam>(test_vect, test_vect));

  Vector_3 arbitrary_vect(1.0, 1.0, 1.0);
  Vector_3 perpendicular_vect(CGAL::cross_product(test_vect, arbitrary_vect));
  EXPECT_FALSE(isHeadOn<TypeParam>(test_vect, perpendicular_vect));

  Vector_3 opposite_vect(-1.0 * test_vect);
  EXPECT_TRUE(isHeadOn<TypeParam>(test_vect, opposite_vect));
}

TYPED_TEST(GeomUtilsTests, getPlane) { getPlane<TypeParam>(this->_test_triangle); }

TYPED_TEST(GeomUtilsTests, getNormal)
{
  EXPECT_THREE_TUPLE_EQ(getNormal<TypeParam>(this->_test_triangle), 0, 0, 6)
}

TYPED_TEST(GeomUtilsTests, getArea) { ASSERT_FT_EQ(getArea<TypeParam>(this->_test_triangle), 3); }

TYPED_TEST(GeomUtilsTests, rawViewFactorKernel)
{
  // Testing a few kernel values which can be derived analytically
  typename TypeParam::Vector_3 from_normal(0.0, 0.0, 1.0);
  typename TypeParam::Vector_3 to_normal(0.0, 0.0, -1.0);
  ASSERT_DOUBLE_EQ(rawViewFactorKernel<TypeParam>({0.0, 0.0, 1.0}, from_normal, to_normal),
                   (1.0 / M_PI));
  ASSERT_DOUBLE_EQ(rawViewFactorKernel<TypeParam>({0.0, 1.0, 1.0}, from_normal, to_normal),
                   (1.0 / (4.0 * M_PI)));
  ASSERT_DOUBLE_EQ(rawViewFactorKernel<TypeParam>({1.0, 1.0, 1.0}, from_normal, to_normal),
                   (1.0 / (9.0 * M_PI)));
}
