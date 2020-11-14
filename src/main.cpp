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
    if (pair->value == 0) {
      continue;
    }
    clique->Pair(pair->getId1(), pair->getId2());
  }
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
  PairsToClique(pairs, clique);
  auto entries = clique->getEntries();
  auto unique = RemoveDup(entries);
  auto file = FileSystem::join(cwd, params.outName);
  if (params.outType == "pairs") {
    CSV::WriteCSVPairs(file, unique);
  } else if (params.outType == "all") {
    CSV::WriteCSV(file, unique);
  }
  for (auto i = 0; i < pairs->getLength(); i++) {
    auto pair = (*pairs)[i];
    delete pair;
  }
  delete pairs;
  delete clique;
  return 0;
}