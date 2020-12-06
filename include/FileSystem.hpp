#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "List.hpp"
#include <string>
#include <functional>
#include "FastVector.hpp"

namespace FileSystem {
  void listContents(const std::string& path, char type, FastVector<std::string>& files); // type = 'd' fro directories and 'f' for files
  bool isDirectory(const std::string& path);
  std::string join(const std::string& a, const std::string& b);
  void getAllFiles(const std::string& path, FastVector<std::string>& files);
  void readFile(const std::string& path, std::function<void(std::string&)> func);
}

#endif