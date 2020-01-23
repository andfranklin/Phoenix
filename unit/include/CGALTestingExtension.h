#include "gtest/gtest.h"
#include "GeometryUtils.h"

#define TYPED_TEST_CASE_SETUP(case_name)                                                           \
  typedef testing::Types<Apprx, Exact> MyTypes;                                                    \
  TYPED_TEST_CASE(case_name, MyTypes);

#define TO_REAL(number) toReal<typename TypeParam::FT>(number)

#define EXPECT_FT_EQ(actual_number, expected_number)                                               \
  EXPECT_DOUBLE_EQ(TO_REAL(actual_number), expected_number)

#define ASSERT_FT_EQ(actual_number, expected_number)                                               \
  ASSERT_DOUBLE_EQ(TO_REAL(actual_number), expected_number)

#define EXPECT_FT_NEAR(actual_number, expected_number, abs_error)                                  \
  EXPECT_NEAR(TO_REAL(actual_number), expected_number, abs_error)

#define ASSERT_FT_NEAR(actual_number, expected_number, abs_error)                                  \
  ASSERT_NEAR(TO_REAL(actual_number), expected_number, abs_error)

#define THREE_TUPLE_ERROR_STR(three_tuple, X, Y, Z)                                                \
  "received (" << TO_REAL(three_tuple.x()) << ", " << TO_REAL(three_tuple.y()) << ", "             \
               << TO_REAL(three_tuple.z()) << "); expected (" << X << ", " << Y << ", " << Z       \
               << ")"

#define EXPECT_THREE_TUPLE_EQ(three_tuple, X, Y, Z)                                                \
  EXPECT_FT_EQ(three_tuple.x(), X) << THREE_TUPLE_ERROR_STR(three_tuple, X, Y, Z);                 \
  EXPECT_FT_EQ(three_tuple.y(), Y) << THREE_TUPLE_ERROR_STR(three_tuple, X, Y, Z);                 \
  EXPECT_FT_EQ(three_tuple.z(), Z) << THREE_TUPLE_ERROR_STR(three_tuple, X, Y, Z);

#define EXPECT_BBOX_EQ(bbox, XMIN, XMAX, YMIN, YMAX, ZMIN, ZMAX)                                   \
  EXPECT_DOUBLE_EQ(bbox.xmin(), XMIN) << "unexpected bbox";                                        \
  EXPECT_DOUBLE_EQ(bbox.xmax(), XMAX) << "unexpected bbox";                                        \
                                                                                                   \
  EXPECT_DOUBLE_EQ(bbox.ymin(), YMIN) << "unexpected bbox";                                        \
  EXPECT_DOUBLE_EQ(bbox.ymax(), YMAX) << "unexpected bbox";                                        \
                                                                                                   \
  EXPECT_DOUBLE_EQ(bbox.zmin(), ZMIN) << "unexpected bbox";                                        \
  EXPECT_DOUBLE_EQ(bbox.zmax(), ZMAX) << "unexpected bbox";