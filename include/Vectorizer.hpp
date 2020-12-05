#ifndef VECTORIZER_H
#define VECTORIZER_H

#include <string>
#include "FastVector.hpp"
#include "HashMap.hpp"
#include "Types.hpp"

struct WordInfo {
  int count = 1;
  std::string word;
};

class Vectorizer {
 public:
  Vectorizer();
  void fit(Vector2D sentences);
  void transform(FastVector<std::string>& sentences,
                 FastVector<float>& vectors);
  ~Vectorizer();
  void getVocab(FastVector<WordInfo*>& vec);
 private:
  HashMap<WordInfo*>* vocab;
  int vocabSize = 0;
};

#endif