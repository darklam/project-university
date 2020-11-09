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

void PairsToClique(CustomVector<Pair *> * pairs, Clique* clique){
  for (auto i = 0; i < pairs->getLength(); i++) {
    auto pair = (*pairs)[i];
    if (pair->value == 0) {
      continue;
    }
    clique->Pair(pair->getId1(), pair->getId2());
  }
}

List<Entry<Set*>*>* RemoveDup(List<Entry<Set*>*>* entries){
  HashMap<Set*> dedupe;
  for (auto j = entries->getRoot(); j != nullptr; j = *(j->getNext())) {
    auto val = j->getValue();
    auto item = val->value;
    std::string key = std::to_string((intptr_t) item);
    dedupe.set(key, item);
    delete val;
  }
  delete entries;
  return dedupe.getEntries();
}

int main() {
  int len = 2048;
  char cwd[len];
  getcwd(cwd, len);
  clock_t begin = clock();
  auto path = FileSystem::join(cwd, "W_Dataset.csv");
  auto clique = new Clique();
  auto pairs = CSV::ReadCSV(path);
  PairsToClique(pairs, clique);
  auto entries = clique->getEntries();
  auto unique = RemoveDup(entries);
  auto file = FileSystem::join(cwd, "W_Out_Pairs.csv");
  CSV::WriteCSVPairs(file, unique);
  for (auto i = 0; i < pairs->getLength(); i++) {
    auto pair = (*pairs)[i];
    delete pair;
  }
  delete pairs;
  delete clique;
  return 0;
}