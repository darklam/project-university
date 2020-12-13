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

struct ProgramParams {
  std::string outName = "W_Out_Pairs.csv";
  std::string outType = "pairs";
  std::string inName = "W_Dataset.csv";
};

void PairsToClique(CustomVector<Pair*>* pairs, Clique* clique) {
  for (auto i = 0; i < pairs->getLength(); i++) {
    auto pair = (*pairs)[i];
    // std::cout << pair->getId1() << " " << pair->getId2() << " " << pair->value << std::endl;
    if (pair->value == 0) {
      clique->NegativePair(pair->getId1(), pair->getId2());
    }else{
      clique->Pair(pair->getId1(), pair->getId2());
    }
  }
}

void deleteEntries(List<Entry<Set*>*>* entries){
  for(auto i = entries->getRoot(); i != nullptr; i = *(i->getNext())){
    auto cur = i->getValue();
    auto item = cur->value;
    auto items = item->getItems();
    for (auto j = items->getRoot(); j != nullptr; j = *(j->getNext())) {
      auto val = j->getValue();
      delete val;
    }
    delete items;
    delete item;
    delete cur;
  }
  delete entries;
}

List<Entry<Set*>*>* RemoveDup(List<Entry<Set*>*>* entries) {
  HashMap<Set*> dedupe;
  for (auto j = entries->getRoot(); j != nullptr; j = *(j->getNext())) {
    auto val = j->getValue();
    auto item = val->value;
    std::string key = std::to_string((intptr_t)item);
    dedupe.set(key, item);
    delete val;
  }
  delete entries;
  return dedupe.getEntries();
}

void parseArgs(int argc, char** argv, ProgramParams* params) {
  for (int i = 0; i < argc; i++) {
    if (i == (argc - 1)) {
      continue;
    }
    auto val = std::string(argv[i]);
    if (val == "--out") {
      params->outName = std::string(argv[i + 1]);
    }
    if (val == "--in") {
      params->inName = std::string(argv[i + 1]);
    }
    if (val == "--type") {
      auto type = std::string(argv[i + 1]);
      if (type != "pairs" && type != "all") {
        std::cout << "Valid values for the type argument are: pairs, all\n";
        exit(EXIT_FAILURE);
      }
      params->outType = type;
    }
  }
}

int main(int argc, char** argv) {
  ProgramParams params;
  parseArgs(argc, argv, &params);
  int len = 2048;
  char cwd[len];
  getcwd(cwd, len);
  auto path = FileSystem::join(cwd, params.inName);
  auto clique = new Clique();
  auto pairs = CSV::ReadCSV(path);
  std::cout << "Input done" << std::endl;
  PairsToClique(pairs, clique);
  std::cout << "Pairs to clique done" << std::endl;
  auto matches = clique->getPositiveEntries();
  auto uniquematches = RemoveDup(matches);
  std::cout << "Duplicates done" << std::endl;
  deleteEntries(uniquematches);
  std::cout << "Delete  done" << std::endl;
  auto nomatches = clique->getNegativeEntries();
  auto unique = RemoveDup(nomatches);
  std::cout << "Duplicates1  done" << std::endl;
  deleteEntries(unique);
  std::cout << "Delete1 done" << std::endl;
  for (auto i = 0; i < pairs->getLength(); i++) {
    auto pair = (*pairs)[i];
    delete pair;
  }
  std::cout << "Delete3 done" << std::endl;
  delete pairs;
  delete clique;
  return 0;
}