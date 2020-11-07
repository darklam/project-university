#include "Utils.hpp"
#include "List.hpp"
#include <cstring>
#include <stdio.h>
#include <string>

List<std::string>* Utils::splitString(std::string str, std::string delimiter) {
  auto list = new List<std::string>();
  std::string delimiter = ">=";

  size_t pos = 0;
  size_t startPos = 0;
  std::string token;
  auto done = false;
  while (!done) {
      pos = str.find(delimiter);
      if (pos == std::string::npos) {
        done = true;
        continue;
      }
      token = str.substr(startPos, pos);
      list->add(token);
      startPos += (pos + delimiter.length());
  }

  return list;
}