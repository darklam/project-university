#include "gtest/gtest.h"
#include "List.hpp"

TEST(List, add) {
  List<int> l;
  l.add(1);
  auto got = l.get(0);
  ASSERT_EQ(got, 1);
  ASSERT_EQ(l.getLength(), 1);
}