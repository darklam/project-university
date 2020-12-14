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
#include "FastVector.hpp"
#include "Vectorizer.hpp"
#include "Pairs.hpp"
#include <TextProcessing.hpp>

struct ProgramParams {
  std::string outName = "W_Out_Pairs.csv";
  std::string outType = "pairs";
  std::string inName = "W_Dataset.csv";
};


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
  std::cout << "Getting input..." << std::endl;
  auto pairs = CSV::ReadCSV(path);
  std::cout << "Creating Dataset..." << std::endl;
  auto _pairs = Pairs::PairsToDataset(pairs);
  auto pairEntries = _pairs->getEntries();
  std::cout << pairEntries->getLength() << std::endl;
  // for(int i = 0; i < _pairs->getLength(); i++){
  //   std::cout << _pairs->get(i) << std::endl;
  // }
  Pairs::deleteDatasetEntries(pairEntries);

  delete _pairs;
  return 0;
}