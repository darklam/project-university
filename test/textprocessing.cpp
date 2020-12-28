#include "gtest/gtest.h"
#include "TextProcessing.hpp"

TEST(TextProcessing, getStopWords) {
  FastVector<std::string> words;
  TextProcessing::getStopWords(words);
  ASSERT_NE(words.getLength(), 0);
}

TEST(TextProcessing, tokenize) {
  FastVector<std::string> sentences;
  sentences.append("HelLo there mAtEy");
  sentences.append("        \t Hey how is      the        fArm fam        ");
  auto result = TextProcessing::tokenizePlus(sentences);

  ASSERT_EQ(result->getLength(), 2);
  auto first = result->get(0);
  auto second = result->get(1);

  ASSERT_EQ(first->getLength(), 1);
  ASSERT_EQ(second->getLength(), 3);

  ASSERT_EQ(first->get(0), "matey");

  ASSERT_EQ(second->get(0), "hey");
  ASSERT_EQ(second->get(1), "farm");
  ASSERT_EQ(second->get(2), "fam");
}