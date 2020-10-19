#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "List.hpp"

namespace FileSystem {
  List<char*>* listContents(const char* path, char type); // type = 'd' fro directories and 'f' for files
  bool isDirectory(const char* path);
  char* join(const char* a, const char* b);
}

#endif