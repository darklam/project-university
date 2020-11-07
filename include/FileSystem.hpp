#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "List.hpp"
#include <string>

namespace FileSystem {
  List<std::string>* listContents(std::string path, char type); // type = 'd' fro directories and 'f' for files
  bool isDirectory(std::string path);
  std::string join(std::string a, std::string b);
  List<std::string>* getAllFiles(std::string path);
}

#endif