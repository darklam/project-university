#ifndef VECTORIZER_H
#define VECTORIZER_H

#include <string>
#include "FastVector.hpp"
#include "HashMap.hpp"
#include "Types.hpp"

struct WordInfo {
  float idf;
  int index;
};

class Vectorizer {
 public:
  Vectorizer();
  void fit(Vector2D sentences);
  void transform(Vector2D sentences,
                 float** vectors);
  ~Vectorizer();
  void getVocab(FastVector<Entry<WordInfo*>*>& vec);
  int getVocabSize();
 private:
  HashMap<WordInfo*>* vocab;
  int vocabSize = 0;
};

#endif