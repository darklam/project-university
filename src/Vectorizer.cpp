#include "Vectorizer.hpp"
#include <cmath>
#include <mutex>
#include <thread>
#include "Set.hpp"

Vectorizer::Vectorizer() {
  this->vocab = new HashMap<WordInfo*>();
}

Vectorizer::~Vectorizer() {
  FastVector<WordInfo*> values(this->vocabSize);
  this->vocab->values(values);
  for (int i = 0; i < values.getLength(); i++) {
    delete values[i];
  }
  delete this->vocab;
}

void Vectorizer::fit(Vector2D sentences) {
  // Make sure those threads NEVER write to the sentences argument
  // or else we're going down like Bloomberg's campaign
  auto coreCount = std::thread::hardware_concurrency();
  std::thread handles[coreCount];
  std::mutex m;
  std::mutex termFreqMutex;
  HashMap<int> termFreq;
  for (int i = 0; i < coreCount; i++) {
    handles[i] = std::thread([&sentences, this, &m, coreCount, i, &termFreqMutex, &termFreq]() {
      int startIndex, endIndex;
      Utils::getBatchIndex(&startIndex, &endIndex, sentences->getLength(), coreCount, i);
      for (int k = startIndex; k < endIndex; k++) {
        auto sentence = (*sentences)[k];
        auto sentenceLen = sentence->getLength();
        Set visited;
        for (int j = 0; j < sentenceLen; j++) {
          auto word = (*sentence)[j];
          HashResult<WordInfo*> res;
          m.lock();
          this->vocab->get(word, &res);
          m.unlock();
          if (!visited.exists(word)) {
            visited.add(word);
            HashResult<int> res;
            termFreqMutex.lock();
            termFreq.get(word, &res);
            termFreq.set(word, res.hasValue ? res.value + 1 : 1);
            termFreqMutex.unlock();
          }

          if (!res.hasValue) {
            auto info = new WordInfo();
            m.lock();
            this->vocab->set(word, info);
            this->vocabSize++;
            m.unlock();
          }
        }
      }
    });
  }

  for (int i = 0; i < coreCount; i++) {
    handles[i].join();
  }

  float documentsCount = sentences->getLength();
  FastVector<Entry<int>*> entries;
  termFreq.getEntries(entries);
  std::mutex vocabMutex;

  for (int core = 0; core < coreCount; core++) {
    handles[core] = std::thread([&documentsCount, &entries, &vocabMutex, core, &coreCount, this]() {
      int start, end;
      Utils::getBatchIndex(&start, &end, entries.getLength(), coreCount, core);
      for (int i = start; i < end; i++) {
        auto entry = *entries[i];
        auto idf = documentsCount / entry.value;
        HashResult<WordInfo*> res;
        vocabMutex.lock();
        this->vocab->get(entry.key, &res);
        if (!res.hasValue) {
          std::cout << "Bruuuuuh this word is not in the vocab?\n";
          return;
        }
        res.value->idf = log(idf);
        vocabMutex.unlock();
        delete entries[i];
      }
    });
  }

  for (int i = 0; i < coreCount; i++) {
    handles[i].join();
  }

}

void Vectorizer::getVocab(FastVector<Entry<WordInfo*>*>& vec) {
  this->vocab->getEntries(vec);
}

void Vectorizer::transform(Vector2D sentences, FastVector<FastVector<float>*>& vectors) {
  auto coreCount = std::thread::hardware_concurrency();
  std::thread handles[coreCount];
  std::mutex vectorsMutex;
  FastVector<Entry<WordInfo*>*> vocabEntries;
  this->vocab->getEntries(vocabEntries);
  for (int core = 0; core < coreCount; core++) {
    handles[core] = std::thread([&sentences, &coreCount, core, this, &vectorsMutex, &vectors, &vocabEntries]() {
      int start, end;
      Utils::getBatchIndex(&start, &end, sentences->getLength(), coreCount, core);
      for (int i = start; i < end; i++) {
        auto sentence = (*sentences)[i];
        auto sentenceLength = sentence->getLength();
        auto vec = new FastVector<float>(this->vocabSize);
        for (int j = 0; j < vocabEntries.getLength(); j++) {
          auto entry = vocabEntries[j];
          auto word = entry->key;
          auto info = entry->value;
          int occurences = 0;
          for (int k = 0; k < sentenceLength; k++) {
            auto currentWord = (*sentence)[k];
            if (currentWord == word) {
              occurences++;
            }
          }
          auto termFrequency = occurences / (float) sentenceLength;
          auto value = termFrequency * info->idf;
          vec->append(value);
        }
        vectorsMutex.lock();
        vectors.set(i, vec);
        vectorsMutex.unlock();
      }
    });
  }
  for (int i = 0; i < coreCount; i++) {
    handles[i].join();
  }
  for (int i = 0; i < vocabEntries.getLength(); i++) {
    delete vocabEntries[i];
  }
}