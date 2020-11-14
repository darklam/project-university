#include "gtest/gtest.h"
#include "FileSystem.hpp"
#include "Set.hpp"

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
  auto result = FileSystem::listContents("../test/listContentsTest", 'f');
  ASSERT_EQ(result->getLength(), 2);
  Set s;
  for (auto i = result->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto val = i->getValue();
    s.add(val);
  }
  ASSERT_EQ(s.exists("file1.txt"), true);
  ASSERT_EQ(s.exists("file2.txt"), true);
  delete result;
  result = FileSystem::listContents("../test/listContentsTest", 'd');
  ASSERT_EQ(result->getLength(), 4);
  Set s2;
  for (auto i = result->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto val = i->getValue();
    s2.add(val);
  }
  ASSERT_EQ(s2.exists("dir1"), true);
  ASSERT_EQ(s2.exists("dir2"), true);
  ASSERT_EQ(s2.exists("."), true);
  ASSERT_EQ(s2.exists(".."), true);
  delete result;
}