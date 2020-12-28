#include "gtest/gtest.h"
#include "TextProcessing.hpp"
#include "Vectorizer.hpp"

TEST(Vectorizer, fit) {
  Vectorizer v;
  FastVector<std::string> sentences;
  sentences.append("This is a great test");
  sentences.append("Believe me I know all the tests and they all tell me this is the best test");
  sentences.append("If you count all the legal lines, this test has won by a great margin");
  auto result = TextProcessing::tokenizePlus(sentences);
  v.fit(result);
  for (int i = 0; i < result->getLength(); i++) {
    delete result->get(i);
  }
  delete result;
  FastVector<Entry<WordInfo*>*> entries;
  v.getVocab(entries);
  ASSERT_EQ(v.getVocabSize(), entries.getLength());
  FastVector<int> indices;
  for (int i = 0; i < entries.getLength(); i++) {
    auto info = entries[i]->value;
    delete entries[i];
    indices.append(info->index);
  }

  // Make sure the indices in the vocab are continuous 
  for (int i = 0; i < entries.getLength(); i++) {
    auto found = indices.includes(i);
    ASSERT_EQ(found, true);
  }
}

TEST(Vectorizer, transform) {
  // We basically just want this not to fail so no asserts lol
  Vectorizer v;
  FastVector<std::string> sentences;
  sentences.append("This is a great test");
  sentences.append("Believe me I know all the tests and they all tell me this is the best test");
  sentences.append("If you count all the legal lines, this test has won by a great margin");
  auto result = TextProcessing::tokenizePlus(sentences);
  v.fit(result);
  for (int i = 0; i < result->getLength(); i++) {
    delete result->get(i);
  }
  delete result;
  FastVector<std::string> sentecesTransform;
  sentecesTransform.append("count all the legal lines the best test");
  auto tokenized = TextProcessing::tokenizePlus(sentecesTransform);
  float** vectors = new float*[1];
  vectors[0] = new float[v.getVocabSize()];
  v.transform(tokenized, vectors);

  delete[] vectors[0];
  delete[] vectors;
}