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
  FastVector<std::string> texts(5);
  texts.append("Hello   there :\":'?;'@!$&^&%*^(*))(*) motherfucker");
  texts.append("What my nam jef jef jef jef my dude   oh m^y");
  texts.append("What  jef dude?");
  texts.append("12  21   hehehe   532^^always jef");
  texts.append("My nam %% ^&^   a                        jef able");
  auto result = TextProcessing::tokenizePlus(texts);
  Vectorizer v;
  v.fit(result);
  FastVector<FastVector<float>*> vectorized(result->getLength());
  vectorized.forceInit(nullptr);
  v.transform(result, vectorized);
  for (int i = 0; i < vectorized.getLength(); i++) {
    auto current = vectorized[i];
    if (current == nullptr) {
      std::cout << "Something went insanely wrong!!!\n";
      continue;
    }
    std::cout << "Sentence: ";
    auto sent = (*result)[i];
    for (int j = 0; j < sent->getLength(); j++) {
      std::cout << (*sent)[j] << " ";
    }
    std::cout << std::endl;
    for (int j = 0; j < current->getLength(); j++) {
      std::cout << (*current)[j] << ", ";
    }
    std::cout << std::endl;
    delete current;
  }
  for (int i = 0; i < result->getLength(); i++) {
    delete (*result)[i];
  }
  delete result;
  FastVector<Entry<WordInfo*>*> vocab;
  v.getVocab(vocab);
  for (int i = 0; i < vocab.getLength(); i++) {
    auto entry = vocab[i];
    auto word = entry->key;
    auto info = entry->value;
    std::cout << "Word: " << word << " - Idf: " << info->idf << std::endl;
    delete entry;
  }
  return 0;
}