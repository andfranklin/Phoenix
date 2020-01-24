#include "gtest/gtest.h"
#include "CGALTestingExtension.h"

#include "SymmetricDualMap.h"
#include <memory>

TEST(SymmetricDualMapTests, basicElementSetting)
{
  SymmetricDualMap<unsigned int, double> mapper;
  mapper(5, 7) = 12.0;

  EXPECT_DOUBLE_EQ(mapper(7, 5), 12.0);
  EXPECT_TRUE(mapper.contains(5, 7));
  EXPECT_TRUE(mapper.contains(7, 5));
  EXPECT_FALSE(mapper.contains(0, 0));
}

TEST(SymmetricDualMapTests, useCase)
{
  SymmetricDualMap<std::shared_ptr<unsigned int>, double> mapper;

  unsigned int a = 12;
  unsigned int b = 16;
  unsigned int c = 20;

  std::shared_ptr<unsigned int> ptr_a(std::make_shared<unsigned int>(a));
  std::shared_ptr<unsigned int> ptr_b(std::make_shared<unsigned int>(b));
  std::shared_ptr<unsigned int> ptr_c(std::make_shared<unsigned int>(c));

  mapper(ptr_a, ptr_b) = 1.0;
  mapper(ptr_b, ptr_c) = 2.0;

  EXPECT_DOUBLE_EQ(mapper(ptr_b, ptr_a), 1.0);
  EXPECT_DOUBLE_EQ(mapper(ptr_c, ptr_b), 2.0);
  EXPECT_FALSE(mapper.contains(ptr_c, ptr_a));
  EXPECT_TRUE(mapper.contains(ptr_b, ptr_a));
}