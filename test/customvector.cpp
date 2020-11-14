#include "gtest/gtest.h"
#include "CustomVector.hpp"

TEST(CustomVector, add) {
  CustomVector<int> vec;
  for (int i = 0; i < 1000; i++) {
    vec.add(i);
  }

  for (int i = 0; i < 1000; i++) {
    auto val = vec[i];
    ASSERT_EQ(val, i);
  }

  ASSERT_EQ(vec.getLength(), 1000);
}