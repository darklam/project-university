#include "gtest/gtest.h"
#include "JSON.hpp"

TEST(JSON, parseCamera) {
  auto camera = JSON::parseJSON("../test/someId/test.json");
  ASSERT_EQ(camera->getId(), "someId//test");
  auto hashMap = camera->getProperties();
  auto entries = hashMap->getEntries();
  ASSERT_EQ(entries->getLength(), 3);
  for (auto i = entries->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto val = i->getValue();
    auto key = val->key;
    auto value = val->value;
    auto mapValue = hashMap->get(key);
    ASSERT_EQ(mapValue->hasValue, true);
    ASSERT_EQ(value, mapValue->value);
    ASSERT_EQ(value->isArray, mapValue->value->isArray);
    if (value->isArray) {
      for (auto i = 0; i < value->arrayValue->getLength(); i++) {
        auto val = (*value->arrayValue)[i];
        auto val2 = (*mapValue->value->arrayValue)[i];
        ASSERT_EQ(val, val2);
      }
    }
    delete mapValue;
    delete val;
  }
  delete entries;
  delete camera;
}

