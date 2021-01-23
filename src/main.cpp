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
#include "TfIdfVectorizer.hpp"
#include "BowVectorizer.hpp"
#include <TextProcessing.hpp>

struct ProgramParams {
  std::string outName = "W_Out_Pairs.csv";
  std::string outType = "pairs";
  std::string inName = "W_Dataset.csv";
  std::string inCameras = "cameras";
};

struct probability {
  std::string str1;
  std::string str2;
  float prob;
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

void train_test_split(FastVector<std::string>& dataset, FastVector<std::string>& train, FastVector<std::string>& test, FastVector<std::string>& validation){
  RandomizeDataset(dataset);
  RandomizeDataset(dataset);
  int test_size = dataset.getLength() * 0.2 + 1;
  int validation_size = dataset.getLength() * 0.2 + 1;
  int train_size = dataset.getLength() * 0.6;
  for(int i = 0; i < test_size; i++){
    test.append(dataset.get(i));
  }
  for(int i = test_size; i < test_size + validation_size; i++){
    validation.append(dataset.get(i));
  }
  for(int i = test_size + validation_size; i < dataset.getLength(); i++){
    train.append(dataset.get(i));
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
    if (val == "--c"){
      params->inCameras = std::string(argv[i + 1]);
    }

  }
}

template<typename T>
int getVector(std::string row, T** vectors, HashMap<int>& ids, float** vector, int max_length){
  std::string tokens[3];
  Utils::splitStringLite(std::string(row), ",", tokens, 3);
  HashResult<int> res1;
  ids.get(tokens[0], &res1);
  if(!res1.hasValue){
    printf("something bad\n");
    exit(1);
  }
  HashResult<int> res2;
  ids.get(tokens[1], &res2);
  if(!res2.hasValue){
    printf("something bad\n");
    exit(1);
  }
  auto v1 = vectors[res1.value];
  auto v2 = vectors[res2.value];
  
  for(int i = 0; i < max_length; i++){
    float min = v1[i] + v2[i];
    min /= 2;
    (*vector)[i] = min;
  }
  return atoi(tokens[2].c_str());
}


template<typename T>
void getDatasetAsVectors(FastVector<std::string>& dataset, T** vectors, 
HashMap<int>& ids, FastVector<float*>& final, FastVector<int>& y_true, int max_features){
  for(int i = 0; i < dataset.getLength(); i++){
    auto row = dataset[i];
    float *vec = new float[max_features];
    int _true = getVector(row, vectors, ids, &vec, max_features);
    final.append(vec);
    y_true.append(_true);
  }
}

void asVector(std::string str1, std::string str2, float** vectors, HashMap<int>& ids, FastVector<float>& result){
  HashResult<int> res1;
  ids.get(str1, &res1);
  if(!res1.hasValue){
    printf("something bad\n");
    exit(1);
  }
  HashResult<int> res2;
  ids.get(str2, &res2);
  if(!res2.hasValue){
    printf("something bad\n");
    exit(1);
  }
  auto v1 = vectors[res1.value];
  auto v2 = vectors[res2.value];
  for(int i = 0; i < 1000; i++){
    result.append(v1[i]);
  }
  for(int i = 0; i < 1000; i++){
    result.append(v2[i]);
  }
}

void resolve(Clique *clique, FastVector<probability*> &new_set){
  for(int i = 0; i < new_set.getLength(); i++){
    auto p = new_set[i];
    if(p->prob >= 0.5){
      clique->setPair(p->str1, p->str2);
    }else{
      clique->NegativePair(p->str1, p->str2);
    }
    delete p;
  }
}



int main(int argc, char** argv) {
  srand(time(NULL));
  ProgramParams params;
  parseArgs(argc, argv, &params);
  int len = 2048;
  char cwd_dataset[len];
  getcwd(cwd_dataset, len);
  auto path_dataset = FileSystem::join(cwd_dataset, params.inName);

  char cwd_cameras[len];
  getcwd(cwd_cameras, len);
  auto path_cameras = FileSystem::join(cwd_cameras, params.inCameras);

  FastVector<CameraDTO*> cameras(30000);
  FastVector<std::string> texts(30000);
  FastVector<std::string> camera_ids(30000);
  HashMap<int> ids(30000);
  
  /* read folder with cameras and extract values */
  std::cout << "Getting cameras..." << std::endl;
  JSON::loadData(path_cameras, cameras);
  for (int i = 0; i < cameras.getLength(); i++) {
    auto str = cameras[i]->getAllProperties();
    texts.append(str);
    camera_ids.append(cameras[i]->getId());
    ids.set(cameras[i]->getId(), i);
    delete cameras[i];
  }

  /*tokenize specs and apply vectorization with max features 1000 */
  std::cout << "Tokenizing...\n";
  auto tokenized = TextProcessing::tokenizePlus(texts);
  TfIdfVectorizer v;
  std::cout << "Fitting the vectorizer...\n";
  v.fit(tokenized, 1000);
  FastVector<Entry<WordInfo*>*> vec;
  v.getVocab(vec);
  int vocab_size = vec.getLength();
  std::cout << "Vocabulary size: " << vec.getLength() << std::endl;
  float** vectors = new float*[texts.getLength()];
  std::cout << "Transforming...\n";
  v.transform(tokenized, vectors);

  /* delete unwanted variables */
  for (int i = 0; i < tokenized->getLength(); i++) {
    delete (*tokenized)[i];
  }
  delete tokenized;
  for(int i = 0; i < vec.getLength(); i++){
    auto entry = vec.get(i);
    delete entry;
  }

  /* read csv */
  std::cout << "Reading w_dataset..." << std::endl;
  auto pairs = CSV::ReadCSV(path_dataset);

  Clique* clique = new Clique();
  auto _pairs = Pairs::PairsToDataset(pairs, params.outType, params.outName, clique);
  FastVector<std::string> dataset(100000);
  _pairs->values(dataset);
  delete _pairs;
  int dataset_size = dataset.getLength();
  std::cout << "Done with dataset, final number of rows: " << dataset_size << std::endl;
  FastVector<std::string> train_set(300000);
  FastVector<std::string> val_set(50000);
  FastVector<std::string> test_set(50000);
  std::cout << "Splitting dataset..." << std::endl;
  train_test_split(dataset, train_set, test_set, val_set);
  std::cout << "Train set size: " << train_set.getLength() << std::endl;
  std::cout << "Test set size: " << test_set.getLength() << std::endl;
  std::cout << "Validation set size: " << val_set.getLength() << std::endl;
  /*--------------------  Part 3 -----------------------------------*/
  Logistic<float> model(vocab_size);
  std::cout << "Fitting model...\n";
  model.fit(train_set, vectors, ids, 0.01);
  float threshold = 0.1;
  float step = 0.5;
  bool flag = true;
  while(threshold < 0.5){
    if(flag){
      flag = false;
      model.fit(train_set, vectors, ids, 0.01);
    }else{
      auto positives = clique->getPositiveEntries();
      auto pos_unique = Pairs::RemoveDup(positives);
      auto negatives = clique->getNegativeEntries();
      auto neg_unique = Pairs::RemoveDup(negatives);
      auto new_pairs = Pairs::createDataset(pos_unique, negatives);
      Pairs::deleteList(neg_unique);
      Pairs::deleteList(pos_unique);
      FastVector<std::string> new_dataset(100000);
      new_pairs->values(dataset);
      delete new_pairs;
      std::cout << "New dataset size: " << new_dataset.getLength() << std::endl;
      model.fit(new_dataset, vectors, ids, 0.01);
    }
    FastVector<probability*> new_set(400000);
    for(int i = 0; i < camera_ids.getLength(); i++){
      for(int j = i + 1; j < camera_ids.getLength(); i++){
        FastVector<float> vec(2000);
        asVector(camera_ids[i], camera_ids[j], vectors, ids, vec);
        auto prob = model.prob(vec);
        if((prob < threshold ) || (prob > 1 - threshold)){
          probability* p = new probability();
          p->str1 = camera_ids[i];
          p->str2 = camera_ids[j];
          p->prob = prob;
          new_set.append(p);
        }
      }
    }
    resolve(clique, new_set);
    threshold += step;
    step *= 2;
  }

  auto positives = clique->getPositiveEntries();
  auto pos_unique = Pairs::RemoveDup(positives);
  auto negatives = clique->getNegativeEntries();
  auto neg_unique = Pairs::RemoveDup(negatives);
  Pairs::deleteEntries(neg_unique);
  Pairs::deleteEntries(pos_unique);
  delete clique;

  std::cout << "Testing...\n";
  FastVector<int> y_true(10000);
  auto pred = model.predict(test_set, vectors, ids, y_true);

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