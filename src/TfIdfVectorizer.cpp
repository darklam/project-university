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

  int index = 0;

  for (int i = 0; i < entriesLength; i++) {
    auto entry = *entries[i];
    HashResult<WordInfo*> res;
    this->vocab->get(entry.key, &res);
    if (!res.hasValue) {
      std::cout << "Bruuuuuh this word is not in the vocab?\n";
      return;
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
  FastVector<Entry<WordInfo*>*> vocabEntries;
  this->vocab->getEntries(vocabEntries);
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

  std::cout << "Extracting top 1000 tf-idf...\n";
  FastVector<float> mean(this->vocabSize);
  for (int word = 0; word < this->vocabSize; word++) {
    float m = 0.0;
    for (int i = 0; i < sentences->getLength(); i++) {
      m += vectors[i][word];
    }
    mean.append(m);
  }
  FastVector<int> positions(1000);
  Sort::sort(1000, mean, positions);
  for (int i = 0; i < sentences->getLength(); i++) {
    float* _vec = new float[1000];
    for (int j = 0; j < 1000; j++) {
      _vec[j] = 0;
    }
    for (int p = 0; p < 1000; p++) {
      _vec[p] = vectors[i][positions[p]];
    }
    delete[] vectors[i];
    vectors[i] = _vec;
  }
  for (int i = 0; i < vocabEntries.getLength(); i++) {
    auto entry = vocabEntries.get(i);
    delete entry;
  }
}

int TfIdfVectorizer::getVocabSize() {
  return this->vocabSize;
}
