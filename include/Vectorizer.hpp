#ifndef VECTORIZER_H
#define VECTORIZER_H

#include <string>
#include "FastVector.hpp"
#include "HashMap.hpp"
#include "Types.hpp"

struct WordInfo {
  float idf;
};

class Vectorizer {
 public:
  Vectorizer();
  void fit(Vector2D sentences);
  void transform(Vector2D sentences,
                 FastVector<FastVector<float>*>& vectors);
  ~Vectorizer();
  void getVocab(FastVector<Entry<WordInfo*>*>& vec);
 private:
  HashMap<WordInfo*>* vocab;
  int vocabSize = 0;
};

#endif