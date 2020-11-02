#include <unistd.h>

#include <iostream>

#include "FileSystem.hpp"
#include "HashMap.hpp"
#include "JSON.hpp"
#include "List.hpp"
#include "Utils.hpp"
#include "CSV.hpp"
#include "Clique.hpp"

int main() {
  int len = 2048;
  char cwd[len];
  getcwd(cwd, len);
  auto path = FileSystem::join(cwd, "y_dataset.csv");
  // auto cameras = JSON::loadData(path);

  // for (auto i = cameras->getRoot(); i != nullptr; i = *(i->getNext())) { 
  //   auto camera = i->getValue();
  //   std::cout << camera->getId() << std::endl;
  //   delete camera;
  // }
  // delete cameras;
  
  // auto pairs = CSV::loadPairs(path);
  // for (auto i = pairs->getRoot(); i != nullptr; i = *(i->getNext())) { 
  //   auto pair = i->getValue();
  //   printf("%s  %s\n", pair->getId1(), pair->getId2());
  //   delete pair;
  // }
  // delete pairs;
  
  return 0;
}