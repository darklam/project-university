#include "FileSystem.hpp"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include "Utils.hpp"
#include "List.hpp"
#include <string>
#include <iostream>

bool FileSystem::isDirectory(const std::string& path) {
  struct stat path_stat;
  stat(path.c_str(), &path_stat);
  return !S_ISREG(path_stat.st_mode);
}

std::string FileSystem::join(const std::string& a, const std::string& b) {
  std::string fin;
  fin.append(a).append("/").append(b);
  return fin;
}


List<std::string>* FileSystem::listContents(const std::string& path, char type) {
  DIR* d;
  struct dirent* dir;
  auto l = new List<std::string>();
  d = opendir(path.c_str());
  if (d == NULL) {
    std::cout << "Something happened when listing directory " << path << std::endl;
    exit(EXIT_FAILURE);
  }
  auto finished = false;
  while (!finished) {
    dir = readdir(d);
    if (dir == NULL) {
      finished = true;
      continue;
    }
    auto p = FileSystem::join(path, dir->d_name);
    auto isFolder = FileSystem::isDirectory(p);
    auto check = isFolder;
    if (type == 'f') {
      check = !check; // If we want files, we reverse the check
    }
    if (check) {
      auto curr = std::string(dir->d_name);
      l->add(curr);
    }
  }
  closedir(d);

  return l;
}

List<std::string>* FileSystem::getAllFiles(const std::string& path) {
  auto result = FileSystem::listContents(path, 'd');
  auto files = new List<std::string>();
  for (auto current = result->getRoot(); current != nullptr; current = *(current->getNext())) {
      auto value = current->getValue();
      if (value.compare("..") == 0 || value.compare(".") == 0) {
          continue;
      }
      auto currentPath = FileSystem::join(path, value);
      auto currentFiles = FileSystem::listContents(currentPath, 'f');
      for (auto j = currentFiles->getRoot(); j != nullptr; j = *(j->getNext())) {
          auto val = j->getValue();
          auto fullPath = FileSystem::join(currentPath, val);
          files->add(fullPath);
      }
      delete currentFiles;
  }

  delete result;

  return files;
}