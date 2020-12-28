#ifndef BOWVECTORIZER_HPP
#define BOWVECTORIZER_HPP

#include <string>
#include "FastVector.hpp"
#include "HashMap.hpp"
#include "Types.hpp"


class BowVectorizer {
 public:
  BowVectorizer();
  void fit(Vector2D sentences);
  void transform(Vector2D sentences, int** vectors);
  ~BowVectorizer();
  void getVocab(FastVector<Entry<int>*>& vec);
  int getVocabSize();
 private:
  HashMap<int>* vocab;
  int vocabSize = 0;
};

#endif