#include "Vectorizer.hpp"
#include <cmath>
#include <mutex>
#include <thread>

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
  // The index where each core will stop
  int perCoreData = ceil(sentences->getLength() / (double)coreCount);
  std::mutex m;
  for (int i = 0; i < coreCount; i++) {
    handles[i] = std::thread([&sentences, this, &m, coreCount, perCoreData, i]() {
      int endIndex;
      if (i == coreCount - 1) {
        // The last core will process whatever is left
        endIndex = sentences->getLength();
      } else {
        endIndex = (i + 1) * perCoreData;
      }
      int startIndex = i * perCoreData;
      // If the cores are more than the sentences, then each core will process
      // just one sentence and the remaining ones will just call it quits
      if (perCoreData == 1) {
        startIndex = i;
        endIndex = i + 1;
        if (endIndex > sentences->getLength()) {
          return;
        }
      }
      for (int k = startIndex; k < endIndex; k++) {
        auto sentence = (*sentences)[k];
        auto sentenceLen = sentence->getLength();
        for (int j = 0; j < sentenceLen; j++) {
          auto word = (*sentence)[j];
          HashResult<WordInfo*> res;
          m.lock();
          this->vocab->get(word, &res);
          m.unlock();
          if (res.hasValue) {
            auto info = res.value;
            m.lock();
            info->count++;
            m.unlock();
          } else {
            auto info = new WordInfo();
            info->word = word;
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
}

void Vectorizer::getVocab(FastVector<WordInfo*>& vec) {
  this->vocab->values(vec);
}