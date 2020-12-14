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
#include <TextProcessing.hpp>

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
  int len = 2048;
  char cwd[len];
  getcwd(cwd, len);
  auto path = FileSystem::join(cwd, "cameras");
  FastVector<CameraDTO*> cameras(30000);
  FastVector<std::string> texts(30000);
  JSON::loadData(path, cameras);
  for (int i = 0; i < cameras.getLength(); i++) {
    auto str = cameras[i]->getAllProperties();
    texts.append(str);
    delete cameras[i];
  }
  std::cout << "Tokenizing...\n";
  auto tokenized = TextProcessing::tokenizePlus(texts);
  std::cout << "Tokenized...\n";
  Vectorizer v;
  std::cout << "Fitting the vectorizer...\n";
  v.fit(tokenized);
  std::cout << "Vectorizer fitted...\n";\
  FastVector<Entry<WordInfo*>*> vec;
  v.getVocab(vec);
  std::cout << "Vocab size: " << vec.getLength() << std::endl;
  for (int i = 0; i < vec.getLength(); i++) {
    std::cout << vec[i]->key << std::endl;
  }
  // float** vectors = new float*[texts.getLength()];
  // v.transform(tokenized, vectors);
  // for (int i = 0; i < texts.getLength(); i++) {
  //   delete[] vectors[i];
  // }
  // delete[] vectors;
  for (int i = 0; i < tokenized->getLength(); i++) {
    delete (*tokenized)[i];
  }
  delete tokenized;
  return 0;
}