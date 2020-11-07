#ifndef UTILS_H
#define UTILS_H

#include "List.hpp"

namespace Utils {
  void copyString(const char* src, char** dest);
  bool compareStrings(const char* a, const char* b);
  List<char>* splitString(char* str, const char* delimiter);
  bool stringContains(char* str, const char* search);
  char* append(char* a, char* b, char* between);
}

#endif