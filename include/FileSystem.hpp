#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "List.hpp"
#include <string>
#include <functional>

namespace FileSystem {
  List<std::string>* listContents(const std::string& path, char type); // type = 'd' fro directories and 'f' for files
  bool isDirectory(const std::string& path);
  std::string join(const std::string& a, const std::string& b);
  List<std::string>* getAllFiles(const std::string& path);
  void readFile(std::string& path, std::function<void(std::string&)> func);
}

#endif