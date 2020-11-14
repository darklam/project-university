#include "gtest/gtest.h"
#include "CSV.hpp"

TEST(CSV, ReadsTheDamnFile) {
  auto result = CSV::ReadCSV("../test/test.csv");
  ASSERT_EQ(result->getLength(), 5);
  std::string ids[5][3] = {
    {"123", "213", "1"},
    {"123", "321", "1"},
    {"123", "143", "0"},
    {"123", "323", "1"},
    {"231", "124", "0"}
  };
  for (int i = 0; i < result->getLength(); i++) {
    auto val = (*result)[i];
    auto current = ids[i];
    ASSERT_EQ(current[0], val->id1);
    ASSERT_EQ(current[1], val->id2);
    ASSERT_EQ(current[2], std::to_string(val->value));
    delete val;
  }
  delete result;
}