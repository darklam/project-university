#include "Utils.hpp"

#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <sstream>
#include <string>
#include "CustomVector.hpp"
#include "FastVector.hpp"

void Utils::splitString(const std::string& str,
                                              const std::string& delimiter,
                                              FastVector<std::string>& tokens) {
  std::size_t current, previous = 0;
  current = str.find(delimiter);
  while (current != std::string::npos) {
    tokens.append(str.substr(previous, current - previous));
    previous = current + 1;
    current = str.find(delimiter, previous);
  }
  tokens.append(str.substr(previous, current - previous));

}

void Utils::splitStringLite(const std::string& str,
                            const std::string& delim,
                            std::string* tokens,
                            int len) {
  std::size_t current, previous = 0;
  current = str.find(",");
  for (int i = 0; i < len; i++) {
    tokens[i] = str.substr(previous, current - previous);
    previous = current + 1;
    current = current = str.find(",", previous);
  }
}

void Utils::splitNew(const std::string& str,
                     const std::string& delimiter,
                     FastVector<std::string>& vec) {
  std::size_t current, previous = 0;
  current = str.find(delimiter);
  while (current != std::string::npos) {
    vec.append(str.substr(previous, current - previous));
    previous = current + 1;
    current = str.find(delimiter, previous);
  }
  vec.append(str.substr(previous, current - previous));
}

std::string Utils::trimString(std::string& str) {
  char tokens[] = {' ', '\n', '\t'};
  int len = sizeof(tokens) / sizeof(tokens[0]);
  size_t first = 0;
  size_t last = str.length();
  size_t check = first;
  bool finished = false;
  while (!finished) {
    auto curr = str.at(first);
    auto found = false;
    for (int i = 0; i < len; i++) {
      if (tokens[i] == curr) {
        first++;
        found = true;
        break;
      }
    }
    if (!found || first >= str.length()) {
      finished = true;
    }
  }
  finished = false;
  while (!finished) {
    auto curr = str.at(last - 1);
    auto found = false;
    for (int i = 0; i < len; i++) {
      if (tokens[i] == curr) {
        last--;
        found = true;
        break;
      }
    }
    if (!found || last <= 0) {
      finished = true;
    }
  }
  return str.substr(first, last - first);
}

void Utils::makeLowercase(std::string& str) {
  std::transform(str.begin(), str.end(), str.begin(),
                 [](unsigned char c) { return std::tolower(c); });
}

void Utils::lowerAndClean(std::string& str) {
  str.erase(std::remove_if(
                str.begin(), str.end(),
                [](char c) { return !(std::isalnum(c) || std::isspace(c)); }),
            str.end());
  str.erase(
      std::unique(str.begin(), str.end(),
                  [](char a, char b) { return (a == b) && (std::isspace(a)); }),
      str.end());
  if (str[str.length() - 1] == ' ') {
    str.erase(str.length() - 1, 1);
  }
  if (str[0] == ' ') {
    str.erase(0, 1);
  }
  Utils::makeLowercase(str);
}

void Utils::getBatchIndex(int* start, int* end, int length, int coreCount, int currentCore) {
  int perCoreData = ceil(length / (double)coreCount);
  if (currentCore == coreCount - 1) {
    // The last core will process whatever is left
    *end = length;
  } else {
    *end = (currentCore + 1) * perCoreData;
  }
  *start = currentCore * perCoreData;
  // If the cores are more than the sentences, then each core will process
  // just one sentence and the remaining ones will just call it quits
  if (perCoreData == 1) {
    *start = currentCore;
    *end = currentCore + 1;
    if (*end > length) {
      *start = -1;
      *end = -1;
      return;
    }
  }
}