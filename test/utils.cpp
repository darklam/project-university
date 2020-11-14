#include "gtest/gtest.h"
#include "Utils.hpp"


TEST(Utils, SplitsTheDamnString) {
  std::string str = "1,2,3,4,5,6";
  auto result = Utils::splitString(str, ",");
  ASSERT_EQ(result->getLength(), 6);
  for (int i = 0; i < result->getLength(); i++) {
    auto val = (*result)[i];
    auto expected = std::to_string(i + 1);
    ASSERT_EQ(val, expected);
  }
  delete result;
}

TEST(Utils, SplitTheDamnStringLite) {
  std::string str = "1,2,3,4,5,6";
  std::string results[6];
  Utils::splitStringLite(str, ",", results, 6);
  for (int i = 0; i < 6; i++) {
    ASSERT_EQ(results[i], std::to_string(i + 1));
  }
}
