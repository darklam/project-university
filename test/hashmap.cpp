#include "gtest/gtest.h"
#include "HashMap.hpp"

TEST(HashMap, GetAndSet) {
  HashMap<int> m;
  for (int i = 0; i < 150; i++) {
    m.set(std::to_string(i), i);
  }
  for (int i = 0; i < 150; i++) {
    auto val = m.get(std::to_string(i));
    ASSERT_EQ(val->hasValue, true);
    ASSERT_EQ(val->value, i);
  }
  
  for (int i = 0; i < 150; i++) {
    m.set(std::to_string(i), 150 - i);
  }

  for (int i = 0; i < 150; i++) {
    auto val = m.get(std::to_string(i));
    ASSERT_EQ(val->hasValue, true);
    ASSERT_EQ(val->value, 150 - i);
  }
}

TEST(HashMap, Collisions) {
  HashMap<int> m(2);
  for (int i = 0; i < 1000; i++) {
    m.set(std::to_string(i), i);
  }
  for (int i = 0; i < 1000; i++) {
    auto v = m.get(std::to_string(i));
    ASSERT_EQ(v->hasValue, true);
    ASSERT_EQ(v->value, i);
  }
}

TEST(HashMap, GetEntries) {
  HashMap<int> m;
  for (int i = 0; i < 100; i++) {
    m.set(std::to_string(i), i);
  }

  auto entries = m.getEntries();
  ASSERT_EQ(entries->getLength(), 100);
  int index = 0;
  for (auto i = entries->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto val = i->getValue();
    auto key = val->key;
    auto num = val->value;
    ASSERT_EQ(std::to_string(num), key);
    delete val;
  }
  delete entries;
}