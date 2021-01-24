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

#define BATCH_SIZE 1024

struct ProgramParams
{
  std::string outName = "W_Out_Pairs.csv";
  std::string outType = "pairs";
  std::string inName = "W_Dataset.csv";
  std::string inCameras = "cameras";
};

struct probability
{
  std::string str1;
  std::string str2;
  float prob;
};

void balanceDataset(FastVector<std::string> &dataset, FastVector<std::string> &train, int size)
{
  int positives = 0;
  int negatives = 0;
  for (int i = 0; i < size; i++){
    auto row = dataset.get(i);
    if (row.back() == '1')
    {
      train.append(row);
      positives++;
    }
    else
    {
      if (positives >= negatives + 1)
      {
        train.append(row);
        negatives++;
      }
    }
  }
}

void RandomizeDataset(FastVector<std::string> &dataset)
{
  int total_size = dataset.getLength();
  int pos1;
  int pos2;
  for (int i = 0; i < total_size; i++){
    pos1 = rand() % total_size;
    pos2 = rand() % total_size;
    if (pos1 == pos2)
    {
      continue;
    }
    auto temp = dataset.get(pos1);
    dataset.set(pos1, dataset.get(pos2));
    dataset.set(pos2, temp);
  }
}

void train_test_split(FastVector<std::string> &dataset, FastVector<std::string> &train, FastVector<std::string> &test, FastVector<std::string> &validation)
{
  RandomizeDataset(dataset);
  RandomizeDataset(dataset);
  int test_size = dataset.getLength() * 0.2 + 1;
  int validation_size = dataset.getLength() * 0.2 + 1;
  int train_size = dataset.getLength() * 0.6;
  for (int i = 0; i < test_size; i++){
    test.append(dataset.get(i));
  }
  for (int i = test_size; i < test_size + validation_size; i++){
    validation.append(dataset.get(i));
  }
  for (int i = test_size + validation_size; i < dataset.getLength(); i++){
    train.append(dataset.get(i));
  }
}

void parseArgs(int argc, char **argv, ProgramParams *params)
{
  for (int i = 0; i < argc; i++){
    if (i == (argc - 1))
    {
      continue;
    }
    auto val = std::string(argv[i]);
    if (val == "--out")
    {
      params->outName = std::string(argv[i + 1]);
    }
    if (val == "--in")
    {
      params->inName = std::string(argv[i + 1]);
    }
    if (val == "--type")
    {
      auto type = std::string(argv[i + 1]);
      if (type != "pairs" && type != "all")
      {
        std::cout << "Valid values for the type argument are: pairs, all\n";
        exit(EXIT_FAILURE);
      }
      params->outType = type;
    }
    if (val == "--c")
    {
      params->inCameras = std::string(argv[i + 1]);
    }
  }
}

template <typename T>
int getVector(std::string row, T **vectors, HashMap<int> &ids, float **vector, int max_length)
{
  std::string tokens[3];
  Utils::splitStringLite(std::string(row), ",", tokens, 3);
  HashResult<int> res1;
  ids.get(tokens[0], &res1);
  if (!res1.hasValue){
    printf("something bad\n");
    exit(1);
  }
  HashResult<int> res2;
  ids.get(tokens[1], &res2);
  if (!res2.hasValue){
    printf("something bad\n");
    exit(1);
  }
  auto v1 = vectors[res1.value];
  auto v2 = vectors[res2.value];

  for (int i = 0; i < max_length; i++){
    float min = v1[i] + v2[i];
    min /= 2;
    (*vector)[i] = min;
  }
  return atoi(tokens[2].c_str());
}

template <typename T>
void getDatasetAsVectors(FastVector<std::string> &dataset, T **vectors,
                         HashMap<int> &ids, FastVector<float *> &final, FastVector<int> &y_true, int max_features)
{
  for (int i = 0; i < dataset.getLength(); i++){
    auto row = dataset[i];
    float *vec = new float[max_features];
    int _true = getVector(row, vectors, ids, &vec, max_features);
    final.append(vec);
    y_true.append(_true);
  }
}

void asVector(std::string str1, std::string str2, float **vectors, HashMap<int> &ids, FastVector<float> &result)
{
  HashResult<int> res1;
  ids.get(str1, &res1);
  if (!res1.hasValue){
    printf("something bad\n");
    exit(1);
  }
  HashResult<int> res2;
  ids.get(str2, &res2);
  if (!res2.hasValue){
    printf("something bad\n");
    exit(1);
  }
  auto v1 = vectors[res1.value];
  auto v2 = vectors[res2.value];
  for (int i = 0; i < 1000; i++){
    result.append(v1[i]);
  }
  for (int i = 0; i < 1000; i++){
    result.append(v2[i]);
  }
}

void resolve(Clique *clique, std::string path)
{
  FILE *file = fopen(path.c_str(), "r");
  if (!file){
    printf("Shit just got real, file: %s\n", path.c_str());
    exit(EXIT_FAILURE);
  }
  bool finished = false;
  char *line = nullptr;
  size_t len = 0;
  while (!finished){
    int length = getline(&line, &len, file);
    if (length == -1)
    {
      finished = true;
      continue;
    }
    std::string tokens[3];
    Utils::splitStringLite(std::string(line), ",", tokens, 3);
    if (tokens[0].length() == 0 || tokens[1].length() == 0)
    {
      printf("Something really bad happened\n");
    }
    std::string str1 = tokens[0];
    std::string str2 = tokens[1];
    if (atoi(tokens[2].c_str()) == 1)
    {
      clique->setPair(str1, str2);
    }
    else
    {
      clique->NegativePair(str1, str2);
    }
  }
  if (line != nullptr){
    free(line);
  }
  fclose(file);
}

bool checkIfExists(std::string str1, std::string str2, HashMap<int> &existing){
  HashResult<int> res1;
  existing.get(str1 + str2, &res1);
  HashResult<int> res2;
  existing.get(str2 + str1, &res2);
  if (res1.hasValue || res2.hasValue)
    return true;
  return false;
}

void iterative_learning(Clique *clique,
                        Logistic<float> &model,
                        FastVector<std::string> camera_ids,
                        FastVector<std::string> dataset,
                        HashMap<int> &existing,
                        HashMap<int> &ids,
                        float **vectors)
{
  float threshold = 0.01;
  float step = 0.5;
  bool flag = true;
  while (threshold < 0.5){
    if (flag){
      flag = false;
      int b = dataset.getLength() / BATCH_SIZE;
      int remaining = dataset.getLength() % BATCH_SIZE;
      FastVector<int> batches(b + 1);
      for (int i = 0; i < b; i++){
        batches.append(BATCH_SIZE);
      }
      batches.append(remaining);
      model.fit(dataset, vectors, ids, batches, 0.01);
    }else{
      auto positives = clique->getPositiveEntries();
      auto pos_unique = Pairs::RemoveDup(positives);
      auto negatives = clique->getNegativeEntries();
      auto new_pairs = Pairs::createDataset(pos_unique, negatives, existing);
      auto neg_unique = Pairs::RemoveDup(negatives);
      Pairs::deleteList(neg_unique);
      Pairs::deleteList(pos_unique);
      FastVector<std::string> new_dataset(100000);
      new_pairs->values(dataset);
      delete new_pairs;
      std::cout << "New dataset size: " << new_dataset.getLength() << std::endl;

      int b = dataset.getLength() / BATCH_SIZE;
      int remaining = dataset.getLength() % BATCH_SIZE;
      FastVector<int> batches(b + 1);
      for (int i = 0; i < b; i++){
        batches.append(BATCH_SIZE);
      }
      batches.append(remaining);
      model.fit(new_dataset, vectors, ids, batches, 0.01);
    }
    // FastVector<probability*> new_set(400000);
    FILE *file;
    file = fopen("nes_set.csv", "w+");
    for (int i = 0; i < 1000; i++){
      for (int j = i + 1; j < 1000; j++){
        if (checkIfExists(camera_ids[i], camera_ids[j], existing)){
          continue;
        }
        FastVector<float> vec(2000);
        asVector(camera_ids[i], camera_ids[j], vectors, ids, vec);
        auto prob = model.prob(vec);
        if ((prob < threshold) || (prob > 1 - threshold)){
          std::cout << prob << " " << " " << threshold << " " << 1 - threshold << std::endl;
          if (prob >= 0.5){
            fprintf(file, "%s, %s, %d\n", camera_ids[i].c_str(), camera_ids[j].c_str(), 1);
          }else{
            fprintf(file, "%s, %s, %d\n", camera_ids[i].c_str(), camera_ids[j].c_str(), 0);
          }
        }
      }
    }
    fclose(file);
    resolve(clique, "nes_set.csv");
    threshold += step;
    step *= 2;
  }

  auto positives1 = clique->getPositiveEntries();
  auto pos_unique1 = Pairs::RemoveDup(positives1);
  auto negatives1 = clique->getNegativeEntries();
  auto neg_unique1 = Pairs::RemoveDup(negatives1);
  Pairs::deleteEntries(neg_unique1);
  Pairs::deleteEntries(pos_unique1);
  delete clique;
}

int main(int argc, char **argv){
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

  FastVector<CameraDTO *> cameras(30000);
  FastVector<std::string> texts(30000);
  FastVector<std::string> camera_ids(30000);
  HashMap<int> ids(30000);

  /* read folder with cameras and extract values */
  std::cout << "Getting cameras..." << std::endl;
  JSON::loadData(path_cameras, cameras);
  for (int i = 0; i < cameras.getLength(); i++){
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
  FastVector<Entry<WordInfo *> *> vec;
  v.getVocab(vec);
  int vocab_size = vec.getLength();
  std::cout << "Vocabulary size: " << vec.getLength() << std::endl;
  float **vectors = new float *[texts.getLength()];
  std::cout << "Transforming...\n";
  v.transform(tokenized, vectors);

  /* delete unwanted variables */
  for (int i = 0; i < tokenized->getLength(); i++){
    delete (*tokenized)[i];
  }
  delete tokenized;
  for (int i = 0; i < vec.getLength(); i++){
    auto entry = vec.get(i);
    delete entry;
  }

  /* read csv */
  std::cout << "Reading w_dataset..." << std::endl;
  auto pairs = CSV::ReadCSV(path_dataset);
  Clique *clique = new Clique();
  HashMap<int> existing_pairs(30000);
  std::cout << "Got pairs creating dataset" << std::endl;

  auto _pairs = Pairs::PairsToDataset(pairs, params.outType, params.outName, clique, existing_pairs);
  FastVector<std::string> dataset(100000);
  _pairs->values(dataset);
  delete _pairs;
  int dataset_size = dataset.getLength();
  std::cout << "Done with dataset, final number of rows: " << dataset_size << std::endl;
  FastVector<std::string> train_set(300000);
  FastVector<std::string> val_set(50000);
  FastVector<std::string> test_set(50000);

  // train test split
  std::cout << "Splitting dataset..." << std::endl;
  train_test_split(dataset, train_set, test_set, val_set);
  std::cout << "Train set size: " << train_set.getLength() << std::endl;
  std::cout << "Test set size: " << test_set.getLength() << std::endl;
  std::cout << "Validation set size: " << val_set.getLength() << std::endl;

  // split to batches
  int b = train_set.getLength() / BATCH_SIZE;
  int remaining = train_set.getLength() % BATCH_SIZE;
  FastVector<int> batches(b + 1);
  for (int i = 0; i < b; i++){
    batches.append(BATCH_SIZE);
  }
  batches.append(remaining);

  // Logistic model
  Logistic<float> model(vocab_size);
  std::cout << "Fitting model...\n";
  model.fit(train_set, vectors, ids, batches, 0.01);

  std::cout << "\nTesting train_set...\n";
  FastVector<int> train_labels(10000);
  auto train_pred = model.predict(train_set, vectors, ids, train_labels);
  std::cout << "\nF1: " << Metrics::f1_score(train_labels, train_pred) << std::endl;
  std::cout << "Precision: " << Metrics::precision_score(train_labels, train_pred) << std::endl;
  std::cout << "Recall: " << Metrics::recall_score(train_labels, train_pred) << std::endl;
  std::cout << "Accuracy: " << Metrics::accuracy_score(train_labels, train_pred) << std::endl;
  delete[] train_pred;

  iterative_learning(clique, model, camera_ids, train_set, existing_pairs, ids, vectors);

  std::cout << "\nTesting validation set...\n";
  FastVector<int> val_labels(10000);
  auto val_pred = model.predict(test_set, vectors, ids, val_labels);
  std::cout << "\nF1: " << Metrics::f1_score(val_labels, val_pred) << std::endl;
  std::cout << "Precision: " << Metrics::precision_score(val_labels, val_pred) << std::endl;
  std::cout << "Recall: " << Metrics::recall_score(val_labels, val_pred) << std::endl;
  std::cout << "Accuracy: " << Metrics::accuracy_score(val_labels, val_pred) << std::endl;
  delete[] val_pred;

  std::cout << "\nTesting test set...\n";
  FastVector<int> test_labels(10000);
  auto test_pred = model.predict(test_set, vectors, ids, test_labels);
  std::cout << "\nF1: " << Metrics::f1_score(test_labels, test_pred) << std::endl;
  std::cout << "Precision: " << Metrics::precision_score(test_labels, test_pred) << std::endl;
  std::cout << "Recall: " << Metrics::recall_score(test_labels, test_pred) << std::endl;
  std::cout << "Accuracy: " << Metrics::accuracy_score(test_labels, test_pred) << std::endl;
  delete[] test_pred;
  printf("here\n");
  for (int i = 0; i < texts.getLength(); i++){
    delete[] vectors[i];
  }
  delete[] vectors;
  return 0;
}