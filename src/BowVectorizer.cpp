#include "BowVectorizer.hpp"
#include <cmath>
#include <mutex>
#include <thread>
#include "Set.hpp"

BowVectorizer::BowVectorizer() {
  this->vocab = new HashMap<int>();
}

BowVectorizer::~BowVectorizer() {
  delete this->vocab;
}

void BowVectorizer::fit(Vector2D sentences) {
    float documentsCount = sentences->getLength();
    for (int i = 0; i < sentences->getLength(); i++) {
        auto sentence = (*sentences)[i];
        auto sentenceLen = sentence->getLength();
        for (int j = 0; j < sentenceLen; j++) {
            auto word = (*sentence)[j];
            HashResult<int> res;
            this->vocab->get(word, &res);
            if (!res.hasValue) {
                int index = this->vocabSize++;
                this->vocab->set(word, index);
            }
        }
    }
}

void BowVectorizer::getVocab(FastVector<Entry<int>*>& vec) {
  this->vocab->getEntries(vec);
}

void BowVectorizer::transform(Vector2D sentences, int** vectors) {

    FastVector<Entry<int>*> vocabEntries;
    this->vocab->getEntries(vocabEntries);
    for (int i = 0; i < sentences->getLength(); i++) {
        auto sentence = (*sentences)[i];
        auto sentenceLength = sentence->getLength();
        int* vec = new int[this->vocabSize];
        for (int j = 0; j < this->vocabSize; j++) {
            vec[j] = 0;
        }
        HashMap<int> occurencesMap;
        for (int j = 0; j < sentenceLength; j++) {
            auto word = (*sentence)[j];
            HashResult<int> res;
            occurencesMap.get(word, &res);
            occurencesMap.set(word, res.hasValue ? res.value + 1 : 1);
        }
        for (int j = 0; j < sentenceLength; j++) { 
            auto word = (*sentence)[j];
            HashResult<int> res;
            this->vocab->get(word, &res);
            if (!res.hasValue) {
                std::cout << "This is much very wrong\n";
                exit(EXIT_FAILURE);
            }
            HashResult<int> freq;
            occurencesMap.get(word, &freq);
            vec[res.value] = freq.value;
        }
        vectors[i] = vec;
    }
    for(int i = 0; i < vocabEntries.getLength(); i++){
        auto entry = vocabEntries.get(i);
        delete entry;
    }
}

int BowVectorizer::getVocabSize() {
  return this->vocabSize;
}