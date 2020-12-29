#include <unistd.h>
#include <TextProcessing.hpp>
#include <ctime>
#include <iostream>
#include <ctime>
#include <cstdlib>
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
#include "BowVectorizer.hpp"
#include <TextProcessing.hpp>

struct ProgramParams {
  std::string outName = "W_Out_Pairs.csv";
  std::string outType = "pairs";
  std::string inName = "W_Dataset.csv";
};

void balanceDataset(FastVector<std::string>& dataset, FastVector<std::string>& train, int size){
  int positives = 0;
  int negatives = 0;
  for(int i = 0; i < size; i++){
    auto row = dataset.get(i);
    if(row.back() == '1'){
      train.append(row);
      positives++;
    }else{
      if(positives >= negatives + 1){
        train.append(row);
        negatives++;
      }
    }
  }
}

void RandomizeDataset(FastVector<std::string>& dataset){
  int total_size = dataset.getLength();
  int pos1;
  int pos2;
  for(int i = 0; i < total_size; i++){
    pos1 = rand() % total_size;
    pos2 = rand() % total_size;
    if(pos1 == pos2){
      continue;
    }
    auto temp = dataset.get(pos1);
    dataset.set(pos1, dataset.get(pos2));
    dataset.set(pos2, temp);
  }
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
  srand(time(NULL));
  /*--------------------  Part 1 -----------------------------------*/
  ProgramParams params;
  parseArgs(argc, argv, &params);
  int len = 2048;
  char cwd1[len];
  getcwd(cwd1, len);
  auto path1 = FileSystem::join(cwd1, params.inName);
  std::cout << "Reading w_dataset..." << std::endl;
  auto pairs = CSV::ReadCSV(path1);

  std::cout << "Creating clique and final rows..." << std::endl;
  auto _pairs = Pairs::PairsToDataset(pairs);
  FastVector<std::string> dataset(100000);
  _pairs->values(dataset);
  delete _pairs;
  int dataset_size = dataset.getLength();
  std::cout << "Dataset size: " << dataset_size << std::endl;
  std::cout << "Randomizing Dataset..." << std::endl;
  RandomizeDataset(dataset);
  /*--------------------  Part 2 -----------------------------------*/
  char cwd[len];
  getcwd(cwd, len);
  auto path = FileSystem::join(cwd, "cameras");
  FastVector<CameraDTO*> cameras(30000);
  FastVector<std::string> texts(30000);
  HashMap<int> ids(30000);
  std::cout << "Getting cameras..." << std::endl;
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
  for (int i = 0; i < tokenized->getLength(); i++) {
    delete (*tokenized)[i];
  }
  delete tokenized;
  for(int i = 0; i < vec.getLength(); i++){
    auto entry = vec.get(i);
    delete entry;
  }
  // /*--------------------  Part 3 -----------------------------------*/
  int train_size = (int)(dataset_size * 0.8);
  int test_size = dataset_size - train_size;
  std::cout << "Dataset size: " << dataset_size << std::endl;
  std::cout << "Train size: " << train_size << std::endl;
  std::cout << "Test size: " << test_size << std::endl;
  Logistic<float> model(vocab_size);
  std::cout << "Fitting model...\n";
  model.fit(dataset, vectors, ids, train_size, 0.01, 1);
  std::cout << "Fit finished...\n";
  std::cout << "Testing...\n";
  FastVector<int> y_true(200);
  auto pred = model.predict(dataset, vectors, ids, dataset_size, train_size, y_true);

  std::cout << "\nF1: " << Metrics::f1_score(y_true, pred) << std::endl;
  std::cout << "Precision: " << Metrics::precision_score(y_true, pred) << std::endl;
  std::cout << "Recall: " << Metrics::recall_score(y_true, pred) << std::endl;
  std::cout << "Accuracy: " << Metrics::accuracy_score(y_true, pred) << std::endl;
  delete[] pred;
  for (int i = 0; i < texts.getLength(); i++) {
    delete[] vectors[i];
  }
  delete[] vectors;
  return 0;
}