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

bool FileSystem::isDirectory(const char* path) {
  struct stat path_stat;
  stat(path, &path_stat);
  return !S_ISREG(path_stat.st_mode);
}

char* FileSystem::join(const char* a, const char* b) {
  int len = strlen(a) + strlen(b) +
            2;  // 2 because the / and the null terminator ain't free
  auto final = new char[len];
  sprintf(final, "%s/%s", a, b);
  return final;
}

List<char>* FileSystem::listContents(const char* path, char type) {
  DIR* d;
  struct dirent* dir;
  auto l = new List<char>(true);
  d = opendir(path);
  if (d == NULL) {
    printf("Something happened when listing directory %s\n", path);
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
    delete[] p;
    if (check) {
      char* cur;
      Utils::copyString(dir->d_name, &cur);
      l->add(cur);
    }
  }
  closedir(d);

  return l;
}

List<char>* FileSystem::getAllFiles(const char* path) {
  auto result = FileSystem::listContents(path, 'd');
  auto files = new List<char>(true);
  for (auto current = result->getRoot(); current != nullptr; current = *(current->getNext())) {
      auto value = current->getValue();
      if (value == NULL) {
          printf("Oof\n");
          continue;
      }
      if (Utils::compareStrings(value, "..") || Utils::compareStrings(value, ".")) {
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
      delete[] currentPath;
  }

  delete result;

  return files;
}