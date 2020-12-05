#include "TextProcessing.hpp"
#include "Utils.hpp"
#include <thread>
#include <mutex>
#include <cmath>
#include <stdio.h>
#include <unistd.h>
#include "FileSystem.hpp"


Vector2D TextProcessing::tokenizePlus(FastVector<std::string>& texts) {
  auto coreCount = std::thread::hardware_concurrency();
  FastVector<std::string>* threadData[coreCount];
  for (int i = 0; i < coreCount; i++) {
    int amount = ceil(texts.getLength() / (double) coreCount);
    threadData[i] = new FastVector<std::string>(amount);
  }

  for (int i = 0; i < texts.getLength(); i++) {
    auto data = threadData[i % coreCount];
    data->append(texts[i]);
    std::cout << texts[i] << std::endl;
  }

  std::mutex m;
  std::thread handles[coreCount];
  Vector2D output = new FastVector<FastVector<std::string>*>(texts.getLength());
  FastVector<std::string> stopWords;
  TextProcessing::getStopWords(stopWords);

  for (int i = 0; i < coreCount; i++) {
    auto currData = threadData[i];

    handles[i] = std::thread([&threadData, i, &m, &output, &stopWords]() {
      auto data = threadData[i];
      for (int j = 0; j < data->getLength(); j++) {
        auto str = (*data)[j];
        Utils::lowerAndClean(str);
        int expectedWords = ceil(str.length() / 5.7); // 4.7 is the average word length for english so +1 for spaces
        auto tokens = new FastVector<std::string>(expectedWords);
        Utils::splitNew(str, " ", *tokens);
        auto filtered = tokens->filter([&](std::string it) {
          return !stopWords.includes(it);
        });
        delete tokens;
        m.lock();
        output->append(filtered);
        m.unlock();
      }
    });
  }

  for (int i = 0; i < coreCount; i++) {
    handles[i].join();
    delete threadData[i];
  }
  return output;
}

void TextProcessing::getStopWords(FastVector<std::string>& words) {
  int len = 2048;
  char cwd[len];
  getcwd(cwd, len);
  std::string path = FileSystem::join(cwd, "stop_words.txt");
  FileSystem::readFile(path, [&](std::string& line) {
    auto trimmed = Utils::trimString(line);
    words.append(trimmed);
  });
}