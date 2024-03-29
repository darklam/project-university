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
#include <functional>

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

void FileSystem::listContents(const std::string& path, char type, FastVector<std::string>& files) {
  DIR* d;
  struct dirent* dir;
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
      files.append(curr);
    }
  }
  closedir(d);
}

void FileSystem::getAllFiles(const std::string& path, FastVector<std::string>& files) {
  FastVector<std::string> result(15);
  FileSystem::listContents(path, 'd', result);
  for (int i = 0; i < result.getLength(); i++) {

      auto value = result[i];
      if (value.compare("..") == 0 || value.compare(".") == 0) {
          continue;
      }
      auto currentPath = FileSystem::join(path, value);
      FastVector<std::string> currentFiles(2000);
      FileSystem::listContents(currentPath, 'f', currentFiles);
      for (int j = 0; j < currentFiles.getLength(); j++) {
          auto val = currentFiles[j];
          auto fullPath = FileSystem::join(currentPath, val);
          files.append(fullPath);
      }
  }
}

void FileSystem::readFile(const std::string& path, std::function<void(std::string&)> func) {
  FILE* file = fopen(path.c_str(), "r");
  size_t len = 0;
  if (!file) {
    printf("Shit just got real, file: %s\n", path.c_str());
    exit(EXIT_FAILURE);
  }
  char* ln = nullptr;
  bool finished = false;
  while (!finished) {
    int length = getline(&ln, &len, file);
    if (length == -1) {
      finished = true;
      continue;
    }
    std::string line(ln);
    func(line);
  }

  if (ln != nullptr) {
    free(ln);
  }
  fclose(file);
}