#include "Utils.hpp"
#include "List.hpp"
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

List<char*>* Utils::splitString(char* str, char* delimiter) {
  auto list = new List<char*>();

  auto token = strtok(str, delimiter);
  // we copy the string because otherwise it is just a substring of the str argument
  // and things can get very damn ugly
  while (token != nullptr) {
    char* copy;
    Utils::copyString(token, &copy);
    list->add(copy);
    token = strtok(nullptr, delimiter);
  }

  return list;
}

bool Utils::stringContains(char* str, char* search) {
  return strstr(str, search) != nullptr;
}
