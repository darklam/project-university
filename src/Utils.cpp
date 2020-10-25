#include "Utils.hpp"
#include <cstring>
#include <stdio.h>

void Utils::copyString(char* src, char** dest) {
  *dest = new char[strlen(src) + 1]; // +1 for the null terminator and to sleep calmly today
  strcpy(*dest, src);
}

bool Utils::compareStrings(const char* a, const char* b) {
  int result = strcmp(a, b);
  return result == 0;
}
