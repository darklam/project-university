#include "gtest/gtest.h"
#include "Set.hpp"

TEST(Set, AddsUniqueElements) {
  Set s;
  s.add("hey");
  s.add("hehe");
  s.add("hey");
  ASSERT_EQ(s.exists("hey"), true);
  ASSERT_EQ(s.exists("hehe"), true);
  auto items = s.getItems();
  ASSERT_EQ(items->getLength(), 2);
  for (auto i = items->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto val = i->getValue();
    delete val;
  }
  delete items;
}

TEST(Set, ItemExists) {
  Set s;
  s.add("hey");
  ASSERT_EQ(s.exists("hey"), true);
}

TEST(Set, MergeSets) {
  Set a;
  Set b;
  a.add("1");
  a.add("2");
  b.add("2");
  b.add("3");
  b.add("4");
  a.merge(&b);
  ASSERT_EQ(a.exists("1"), true);
  ASSERT_EQ(a.exists("2"), true);
  ASSERT_EQ(a.exists("3"), true);
  ASSERT_EQ(a.exists("4"), true);
  auto items = a.getItems();
  ASSERT_EQ(items->getLength(), 4);
  for (auto i = items->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto val = i->getValue();
    delete val;
  }
  delete items;
}

TEST(Set, GetItems) {
  Set a;
  for (int i = 0; i < 10; i++) {
    a.add(std::to_string(i));
  }
  Set test; // YES I AM USING MY SET TO TEST MY SET SO WHAT?
  auto items = a.getItems();
  for (auto i = items->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto val = i->getValue();
    auto entryVal = val->value;
    delete val;
    ASSERT_EQ(test.exists(entryVal), false);
    test.add(entryVal);
  }
  delete items;
}