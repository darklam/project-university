#ifndef TFIDFVECTORIZER_H
#define TFIDFVECTORIZER_H

#include <string>
#include "FastVector.hpp"
#include "HashMap.hpp"
#include "Types.hpp"

struct WordInfo {
  float idf;
  int index;
};

class TfIdfVectorizer {
 public:
  TfIdfVectorizer();
  void fit(Vector2D sentences);
  void fit(Vector2D sentences, int max_features);
  void transform(Vector2D sentences,
                 float** vectors);
  ~TfIdfVectorizer();
  void getVocab(FastVector<Entry<WordInfo*>*>& vec);
  int getVocabSize();
 private:
  HashMap<WordInfo*>* vocab;
  int vocabSize = 0;
};

#endif