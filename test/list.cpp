#include "gtest/gtest.h"
#include "List.hpp"

TEST(List, add) {
  List<int> l;
  l.add(1);
  auto got = l.get(0);
  ASSERT_EQ(got, 1);
  ASSERT_EQ(l.getLength(), 1);
}

TEST(List, getLength) {
  List<int> l;
  for (int i = 0; i < 1234; i++) {
    l.add(i);
  }
  ASSERT_EQ(l.getLength(), 1234);
}

TEST(List, get) {
  List<int> l;
  for (int i = 0; i < 1234; i++) {
    l.add(i);
  }
  ASSERT_EQ(l.get(300), 300);
}

TEST(List, Roots) {
  List<int> l;
  for (int i = 0; i < 100; i++) {
    l.add(i);
  }
  int index = 0;
  for (auto i = l.getRoot(); i != nullptr; i = *(i->getNext())) {
    auto val = i->getValue();
    ASSERT_EQ(val, index++);
  }
}