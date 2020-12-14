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
  // Part1
  ProgramParams params;
  parseArgs(argc, argv, &params);
  int len = 2048;
  char cwd1[len];
  getcwd(cwd1, len);
  auto path1 = FileSystem::join(cwd1, params.inName);
  std::cout << "Getting input..." << std::endl;
  auto pairs = CSV::ReadCSV(path1);
  std::cout << "Creating Dataset..." << std::endl;
  auto _pairs = Pairs::PairsToDataset(pairs);
  auto pairEntries = _pairs->getEntries();
  int outputSize = pairEntries->getLength();
  std::cout << outputSize << std::endl;
  // Part 2
  char cwd[len];
  getcwd(cwd, len);
  auto path = FileSystem::join(cwd, "cameras");
  FastVector<CameraDTO*> cameras(30000);
  FastVector<std::string> texts(30000);
  JSON::loadData(path, cameras);
  for (int i = 0; i < cameras.getLength(); i++) {
    texts.append(cameras[i]->getAllProperties());
  }
  std::cout << "Tokenizing...\n";
  auto tokenized = TextProcessing::tokenizePlus(texts);
  std::cout << "Tokenized...\n";
  Vectorizer v;
  std::cout << "Fitting the vectorizer...\n";
  v.fit(tokenized);
  std::cout << "Vectorizer fitted...\n";
  float** vectors = new float*[texts.getLength()];
  v.transform(tokenized, vectors);
  std::cout << "Transform ended..." << std::endl;
  HashMap<float*>* map = new HashMap<float*>(30000);
  for(int i = 0; i < cameras.getLength(); i++){
    map->set(cameras[i]->getId(), vectors[i]);
  }
  std::cout << "Creating final dataset" << std::endl;
  for (int i = 0; i < texts.getLength(); i++) {
    delete cameras[i];
    delete[] vectors[i];
  }
  delete[] vectors;
  for (int i = 0; i < tokenized->getLength(); i++) {
    delete (*tokenized)[i];
  }
  delete tokenized;
  Pairs::deleteDatasetEntries(pairEntries);
  delete _pairs;
  delete map;
  return 0;
}