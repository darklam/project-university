#include <unistd.h>
#include <iostream>
#include <string>
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
  std::cout << "Done" << std::endl;
  for (int i = 0; i < cameras->getLength(); i++) {
    auto camera = (*cameras)[i];
    std::cout << "ID: " << camera->getId() << std::endl;
    delete camera;
  }
  delete cameras;
  return 0;
}