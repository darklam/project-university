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
  auto path = FileSystem::join(cwd, "datasets/cameras");
  auto cameras = JSON::loadData(path);

  for (auto i = cameras->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto camera = i->getValue();
    printf("%s\n", camera->getId());
    delete camera;
  }
  delete cameras;
  return 0;
}