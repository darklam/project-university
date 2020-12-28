#include "gtest/gtest.h"
#include "FileSystem.hpp"
#include "Set.hpp"
#include "FastVector.hpp"

TEST(FileSystem, JoinsThePath) {
  auto result = FileSystem::join("one-dir", "another-dir");
  ASSERT_EQ(result, "one-dir/another-dir");
}

TEST(FileSystem, IsDirectory) {
  auto isDirectory = FileSystem::isDirectory("../test");
  ASSERT_EQ(isDirectory, true);
  auto shouldNotBe = FileSystem::isDirectory("../test/main.cpp");
  ASSERT_EQ(shouldNotBe, false);
}

TEST(FileSystem, ListContents) {
  FastVector<std::string> result;
  FileSystem::listContents("../test/listContentsTest", 'f', result);
  ASSERT_EQ(result.getLength(), 2);
  Set s;
  for (auto i = 0; i < result.getLength(); i++) {
    auto val = result[i];
    s.add(val);
  }
  ASSERT_EQ(s.exists("file1.txt"), true);
  ASSERT_EQ(s.exists("file2.txt"), true);
  FastVector<std::string> result2;
  FileSystem::listContents("../test/listContentsTest", 'd', result2);
  ASSERT_EQ(result2.getLength(), 4);
  Set s2;
  for (auto i = 0; i < result2.getLength(); i++) {
    auto val = result2[i];
    s2.add(val);
  }
  ASSERT_EQ(s2.exists("dir1"), true);
  ASSERT_EQ(s2.exists("dir2"), true);
  ASSERT_EQ(s2.exists("."), true);
  ASSERT_EQ(s2.exists(".."), true);
}