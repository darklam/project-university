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
  for (int i = 0; i < str.length(); i++) {
    str[i] = std::tolower(str[i]);
  }
}

std::string Utils::removeSpecial(std::string& str) {
  std::string out = "";
  out.reserve(str.length());
  for (int i = 0; i < str.length(); i++) {
    auto c = str[i];
    if (std::isalnum(c) || std::isspace(c)) {
      out += c;
    }
  }

  return out;
}

std::string Utils::fixWhitespace(std::string& str) {
  if (str.length() == 0) {
    return "";
  }
  int first = -1;
  for (int i = 0; i < str.length(); i++) {
    if (std::isspace(str[i])) {
      first = i;
      break;
    }
  }
  if (first == -1) {
    return std::string(str);
  }
  std::string out = "";
  auto len = str.length();
  out.reserve(len);
  bool flag = false;
  for (int i = 0; i < len; i++) {
    if (std::isspace(str[i])) {
      if (!flag) {
        out += str[i];
        flag = true;
      }
    } else {
      flag = false;
      out += str[i];
    }
  }
  return out;
}

void Utils::lowerAndClean(std::string& str) {
  auto curr = Utils::fixWhitespace(str);
  curr = Utils::removeSpecial(curr);
  if (curr[curr.length() - 1] == ' ') {
    curr.erase(curr.length() - 1, 1);
  }
  if (curr[0] == ' ') {
    curr.erase(0, 1);
  }
  Utils::makeLowercase(curr);
  str.assign(curr);
}

void Utils::getBatchIndex(int* start,
                          int* end,
                          int length,
                          int coreCount,
                          int currentCore) {
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

std::string Utils::getEnvVar(std::string key) {
  auto var = std::getenv(key.c_str());
  if (var == nullptr) {
    return "";
  }
  return std::string(var);
}