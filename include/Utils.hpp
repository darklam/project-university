#ifndef UTILS_H
#define UTILS_H

#include "List.hpp"

namespace Utils {
  void copyString(char* src, char** dest);
  bool compareStrings(const char* a, const char* b);
  List<char*>* splitString(char* str, char* delimiter);
  bool stringContains(char* str, char* search);
}

#endif