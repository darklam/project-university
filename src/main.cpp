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
  auto path = FileSystem::join(cwd, "W_Dataset.csv");
  // auto cameras = JSON::loadData(path);

  // for (auto i = cameras->getRoot(); i != nullptr; i = *(i->getNext())) { 
  //   auto camera = i->getValue();
  //   std::cout << camera->getId() << std::endl;
  //   delete camera;
  // }
  // delete cameras;
  auto clique = new Clique();
  auto pairs = CSV::ReadCSV(path);
  for (auto i = pairs->getRoot(); i != nullptr; i = *(i->getNext())) { 
    auto pair = i->getValue();
    if(pair->getMatch() == 0) continue;
    clique->Add(pair->getId1());
    clique->Add(pair->getId2());
    clique->Pair(pair->getId1(), pair->getId2());
  }
  auto entries = clique->getEntries();
  // for (auto i = entries->getRoot(); i != nullptr; i = *(i->getNext())) { 
  //   auto entry = i->getValue();
  //   // printf("%s\n", entry->key);
  //   auto list = entry->value;
  //   for (auto j = list->getRoot(); j != nullptr; j = *(j->getNext())) { 
  //     printf("%s ", j->getValue());
  //   }
  //   printf("\n");
  // }
  // delete entries;
  CSV::WriteCSV(entries);
  //delete clique;
  return 0;
}