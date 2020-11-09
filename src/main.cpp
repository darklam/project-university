#include <unistd.h>
#include <ctime>
#include <iostream>
#include <string>
#include "CSV.hpp"
#include "Clique.hpp"
#include "FileSystem.hpp"
#include "HashMap.hpp"
#include "JSON.hpp"
#include "List.hpp"
#include "Set.hpp"
#include "Utils.hpp"



int main() {
  int len = 2048;
  char cwd[len];
  getcwd(cwd, len);
  clock_t begin = clock();
  auto path = FileSystem::join(cwd, "W_Dataset.csv");
  auto clique = new Clique();
  auto pairs = CSV::ReadCSV(path);

  HashMap<Set*> map;

  for (auto i = 0; i < pairs->getLength(); i++) {
    auto pair = (*pairs)[i];
    if (pair->value == 0) {
      continue;
    }
    clique->Pair(pair->getId1(), pair->getId2());
  }

  auto res = clique->getEntries();
  printf("exw mpei\n");
  HashMap<Set*> dedupe;
  for (auto j = res->getRoot(); j != nullptr; j = *(j->getNext())) {
    auto val = j->getValue();
    auto item = val->value;
    std::string key = std::to_string((intptr_t) item);
    dedupe.set(key, item);
    delete val;
  }
  delete res;

  auto unique = dedupe.getEntries();
  for (auto i = unique->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto cur = i->getValue();
    auto item = cur->value;
    auto items = item->getItems();
    for (auto i = items->getRoot(); i != nullptr; i = *(i->getNext())) {
      auto val = i->getValue();
      std::cout << val->value << ",";
      delete val;
    }
    std::cout << std::endl;
    delete cur;
  }

  delete unique;

  for (auto i = 0; i < pairs->getLength(); i++) {
    auto pair = (*pairs)[i];
    delete pair;
  }
  delete pairs;
  delete clique;
  return 0;
}