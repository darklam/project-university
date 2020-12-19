#include <unistd.h>
#include <TextProcessing.hpp>
#include <ctime>
#include <iostream>
#include <string>
#include "CSV.hpp"
#include "Clique.hpp"
#include "FastVector.hpp"
#include "FileSystem.hpp"
#include "HashMap.hpp"
#include "JSON.hpp"
#include "List.hpp"
#include "Logistic.hpp"
#include "Metrics.hpp"
#include "Pairs.hpp"
#include "Set.hpp"
#include "Utils.hpp"
#include "Vectorizer.hpp"

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

  FastVector<std::string> dataset(100000);
  _pairs->values(dataset);
  delete _pairs;
  int dataset_size = dataset.getLength();

  /*--------------------  Part 2 -----------------------------------*/
  char cwd[len];
  getcwd(cwd, len);
  auto path = FileSystem::join(cwd, "cameras");
  FastVector<CameraDTO*> cameras(30000);
  FastVector<std::string> texts(30000);
  HashMap<int> ids(30000);
  std::cout << "Getting input..." << std::endl;
  JSON::loadData(path, cameras);
  for (int i = 0; i < cameras.getLength(); i++) {
    auto str = cameras[i]->getAllProperties();
    texts.append(str);
    ids.set(cameras[i]->getId(), i);
    delete cameras[i];
  }
  std::cout << "Tokenizing...\n";
  auto tokenized = TextProcessing::tokenizePlus(texts);
  std::cout << "Tokenized...\n";
  Vectorizer v;
  std::cout << "Fitting the vectorizer...\n";
  v.fit(tokenized);
  std::cout << "Vectorizer fitted...\n";
  FastVector<Entry<WordInfo*>*> vec;
  v.getVocab(vec);
  int vocab_size = vec.getLength();
  std::cout << "Vocab size: " << vec.getLength() << std::endl;
  float** vectors = new float*[texts.getLength()];
  v.transform(tokenized, vectors);
  std::cout << "Transform ended...\n";
  for (int i = 0; i < tokenized->getLength(); i++) {
    delete (*tokenized)[i];
  }
  delete tokenized;

  /*--------------------  Part 3 -----------------------------------*/
  int train_size = (int)(dataset_size * 0.8);
  int test_size = dataset_size - train_size;
  std::cout << "Dataset size: " << dataset_size << std::endl;
  std::cout << "Train size: " << train_size << std::endl;
  std::cout << "Test size: " << test_size << std::endl;
  Logistic<float> model(vocab_size);
  std::cout << "Fitting model...\n";
  model.fit(dataset, vectors, ids, train_size, 0.01);
  FastVector<int> y_true(200);
  auto pred =
      model.predict(dataset, vectors, ids, dataset_size, train_size, y_true);
  std::cout << "F1: " << Metrics::f1_score(y_true, pred) << std::endl;
  std::cout << "Precision: " << Metrics::precision_score(y_true, pred)
            << std::endl;
  std::cout << "Recall: " << Metrics::recall_score(y_true, pred) << std::endl;
  std::cout << "Accuracy: " << Metrics::accuracy_score(y_true, pred)
            << std::endl;
  auto losses = model.getLossHistory();
  for (int i = 0; i < losses->getLength(); i++) {
    std::cout << (*losses)[i] << std::endl;
  }
  delete[] pred;
  for (int i = 0; i < texts.getLength(); i++) {
    delete[] vectors[i];
  }
  delete[] vectors;
  return 0;
}