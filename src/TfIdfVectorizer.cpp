#include "TfIdfVectorizer.hpp"
#include <cmath>
#include <mutex>
#include <thread>
#include "Set.hpp"
#include "Sort.hpp"

TfIdfVectorizer::TfIdfVectorizer() {
  this->vocab = new HashMap<WordInfo*>();
}

TfIdfVectorizer::~TfIdfVectorizer() {
  FastVector<WordInfo*> values(this->vocabSize);
  this->vocab->values(values);
  for (int i = 0; i < values.getLength(); i++) {
    delete values[i];
  }
  delete this->vocab;
}

void TfIdfVectorizer::fit(Vector2D sentences) {
  HashMap<int> termFreq;
  float documentsCount = sentences->getLength();
  for (int i = 0; i < sentences->getLength(); i++) {
    auto sentence = (*sentences)[i];
    auto sentenceLen = sentence->getLength();
    Set visited;
    for (int j = 0; j < sentenceLen; j++) {
      auto word = (*sentence)[j];
      HashResult<WordInfo*> res;
      this->vocab->get(word, &res);
      if (!visited.exists(word)) {
        visited.add(word);
        HashResult<int> res;
        termFreq.get(word, &res);
        termFreq.set(word, res.hasValue ? res.value + 1 : 1);
      }

      if (!res.hasValue) {
        auto info = new WordInfo();
        this->vocabSize++;
        this->vocab->set(word, info);
      }
    }
  }

  FastVector<Entry<int>*> entries;
  termFreq.getEntries(entries);
  int entriesLength = entries.getLength();

  float mean = 0.0;
  for (int i = 0; i < entriesLength; i++) {
    mean += entries[i]->value;
  }

  mean /= entriesLength;

  int index = 0;

  for (int i = 0; i < entriesLength; i++) {
    auto entry = *entries[i];
    HashResult<WordInfo*> res;
    this->vocab->get(entry.key, &res);
    if (!res.hasValue) {
      std::cout << "Bruuuuuh this word is not in the vocab?\n";
      return;
    }
    if (entry.value < mean * 0.2) {
      this->vocab->remove(entry.key);
      delete res.value;
      this->vocabSize--;
      delete entries[i];
      continue;
    }
    auto idf = documentsCount / entry.value;
    res.value->idf = log(idf);
    res.value->index = index++;
    delete entries[i];
  }
}

void TfIdfVectorizer::fit(Vector2D sentences, int max_featues) {
  HashMap<int> termFreq;
  float documentsCount = sentences->getLength();
  for (int i = 0; i < sentences->getLength(); i++) {
    auto sentence = (*sentences)[i];
    auto sentenceLen = sentence->getLength();
    Set visited;
    for (int j = 0; j < sentenceLen; j++) {
      auto word = (*sentence)[j];
      HashResult<WordInfo*> res;
      this->vocab->get(word, &res);
      if (!visited.exists(word)) {
        visited.add(word);
        HashResult<int> res;
        termFreq.get(word, &res);
        termFreq.set(word, res.hasValue ? res.value + 1 : 1);
      }

      if (!res.hasValue) {
        auto info = new WordInfo();
        this->vocabSize++;
        this->vocab->set(word, info);
      }
    }
  }

  FastVector<Entry<int>*> entries;
  termFreq.getEntries(entries);
  FastVector<int> indexes(max_featues);
  Sort::sortMax(max_featues, entries, indexes);
  int entriesLength = entries.getLength();
  int index = 0;
  for (int i = 0; i < entriesLength; i++) {
    auto entry = *entries[i];
    HashResult<WordInfo*> res;
    this->vocab->get(entry.key, &res);
    if (!res.hasValue) {
      std::cout << "Bruuuuuh this word is not in the vocab?\n";
      return;
    }
    if (!indexes.includes(i)) {
      this->vocab->remove(entry.key);
      delete res.value;
      this->vocabSize--;
      delete entries[i];
      continue;
    }
    auto idf = documentsCount / entry.value;
    res.value->idf = log(idf);
    res.value->index = index++;
    delete entries[i];
  }
}

void TfIdfVectorizer::getVocab(FastVector<Entry<WordInfo*>*>& vec) {
  this->vocab->getEntries(vec);
}

void TfIdfVectorizer::transform(Vector2D sentences, float** vectors) {
  for (int i = 0; i < sentences->getLength(); i++) {
    auto sentence = (*sentences)[i];
    auto sentenceLength = sentence->getLength();
    float* vec = new float[this->vocabSize];
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
      HashResult<WordInfo*> res;
      this->vocab->get(word, &res);
      if (!res.hasValue) {
        continue;
      }
      HashResult<int> freq;
      occurencesMap.get(word, &freq);
      vec[res.value->index] = freq.value * res.value->idf;
      vec[res.value->index] /= sentenceLength;
    }
    vectors[i] = vec;
  }
}

int TfIdfVectorizer::getVocabSize() {
  return this->vocabSize;
}