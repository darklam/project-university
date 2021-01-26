#include "TextProcessing.hpp"
#include <stdio.h>
#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include "FileSystem.hpp"
#include "Utils.hpp"

Vector2D TextProcessing::tokenizePlus(FastVector<std::string>& texts) {
  Vector2D output = new FastVector<FastVector<std::string>*>(texts.getLength());

  FastVector<std::string> stopWords(600);
  TextProcessing::getStopWords(stopWords);

  for (int i = 0; i < texts.getLength(); i++) {
    auto str = texts[i];
    Utils::lowerAndClean(str);
    int expectedWords = ceil(str.length() / 5.7);
    auto tokens = new FastVector<std::string>(expectedWords);
    Utils::splitNew(str, " ", *tokens);
    auto filtered =
        tokens->filter([&](std::string it) { return !stopWords.includes(it); });
    delete tokens;
    output->append(filtered);
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