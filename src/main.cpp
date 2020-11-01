#include <unistd.h>

#include <iostream>

#include "FileSystem.hpp"
#include "HashMap.hpp"
#include "JSON.hpp"
#include "List.hpp"
#include "Utils.hpp"

int main() {
  int len = 2048;
  char cwd[len];
  getcwd(cwd, len);
  auto path = FileSystem::join(cwd, "datasets/cameras-test");
  auto cameras = JSON::loadData(path);

  for (int i = 0; i < cameras->getLength(); i++) {
    auto camera = (*cameras)[i];
    printf("%s\n", camera->getId());
  }
  delete cameras;
  return 0;
}