#include <unistd.h>
#include <TextProcessing.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include "BowVectorizer.hpp"
#include "CSV.hpp"
#include "Clique.hpp"
#include "DatasetsFunc.hpp"
#include "FastVector.hpp"
#include "FileSystem.hpp"
#include "HashMap.hpp"
#include "Iterative.hpp"
#include "JSON.hpp"
#include "JobScheduler.hpp"
#include "List.hpp"
#include "Logistic.hpp"
#include "Metrics.hpp"
#include "Pairs.hpp"
#include "Set.hpp"
#include "TfIdfVectorizer.hpp"
#include "Utils.hpp"

#define BATCH_SIZE 1024

struct ProgramParams {
  std::string outName = "W_Out_Pairs.csv";
  std::string outType = "pairs";
  std::string inName = "w_dataset.csv";
  std::string inCameras = "cameras";
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
    if (val == "--c") {
      params->inCameras = std::string(argv[i + 1]);
    }
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
  HashMap<int> ids(30000);

  /* read folder with cameras and extract values */
  std::cout << "Getting cameras..." << std::endl;
  JSON::loadData(path_cameras, cameras);
  std::string* camera_ids = new std::string[cameras.getLength()];
  int total_cameras = cameras.getLength();
  std::cout << "Total cameras: " << total_cameras << std::endl;
  for (int i = 0; i < total_cameras; i++) {
    auto str = cameras[i]->getAllProperties();
    texts.append(str);
    camera_ids[i] = cameras[i]->getId();
    ids.set(cameras[i]->getId(), i);
    delete cameras[i];
  }

  /*tokenize specs and apply vectorization with max features 1000 */
  std::cout << "Tokenizing...\n";
  auto tokenized = TextProcessing::tokenizePlus(texts);
  TfIdfVectorizer v;
  std::cout << "Fitting the vectorizer...\n";
  v.fit(tokenized);
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
  for (int i = 0; i < vec.getLength(); i++) {
    auto entry = vec.get(i);
    delete entry;
  }

  /* read csv */
  std::cout << "Reading w_dataset..." << std::endl;
  auto pairs = CSV::ReadCSV(path_dataset);
  Clique* clique = new Clique();
  HashMap<int> existing_pairs(30000);
  std::cout << "Got pairs creating dataset" << std::endl;

  auto _pairs = Pairs::PairsToDataset(pairs, params.outType, params.outName,
                                      clique, existing_pairs);
  FastVector<std::string> dataset(100000);
  _pairs->values(dataset);
  delete _pairs;
  int dataset_size = dataset.getLength();
  std::cout << "Done with dataset, final number of rows: " << dataset_size
            << std::endl;
  int train_size = 0.6 * dataset.getLength();
  int test_size = 0.2 * dataset.getLength() + 1;
  FastVector<std::string> train_set(train_size);
  FastVector<std::string> val_set(test_size);
  FastVector<std::string> test_set(test_size);

  // train test split
  std::cout << "Splitting dataset..." << std::endl;
  DatasetsFunc::train_test_split(dataset, train_set, test_set, val_set);
  std::cout << "Train set size: " << train_set.getLength() << std::endl;
  std::cout << "Test set size: " << test_set.getLength() << std::endl;
  std::cout << "Validation set size: " << val_set.getLength() << std::endl;

  // split to batches
  int total = train_set.getLength() / BATCH_SIZE;
  FastVector<int> batches(total + 1);
  DatasetsFunc::split_in_batches(batches, train_set.getLength());

  // Logistic model
  Logistic<float>* model = new Logistic<float>(1000);
  std::cout << "Fitting model...\n";
  model->fit(train_set, vectors, ids, batches, 0.01, 5);

  std::cout << "\nTesting train_set...\n";
  FastVector<int> train_labels(train_set.getLength());
  train_labels.forceInit(0);
  auto train_pred = model->predict(train_set, vectors, ids, train_labels);
  Metrics::printMetrics(train_labels, train_pred);
  delete[] train_pred;

  std::cout << "\nApplying iterative learning method..." << std::endl;
  Iterative::train(clique, model, camera_ids, train_set, existing_pairs, ids, vectors, total_cameras);

  auto positives1 = clique->getPositiveEntries();
  auto pos_unique1 = Pairs::RemoveDup(positives1);
  auto negatives1 = clique->getNegativeEntries();
  auto neg_unique1 = Pairs::RemoveDup(negatives1);
  Pairs::deleteEntries(neg_unique1);
  Pairs::deleteEntries(pos_unique1);
  delete clique;

  std::cout << "\nTesting validation set...\n";
  FastVector<int> val_labels(test_set.getLength());
  val_labels.forceInit(0);
  auto val_pred = model->predict(test_set, vectors, ids, val_labels);
  Metrics::printMetrics(val_labels, val_pred);
  delete[] val_pred;

  std::cout << "\nTesting test set...\n";
  FastVector<int> test_labels(test_set.getLength());
  test_labels.forceInit(0);
  auto test_pred = model->predict(test_set, vectors, ids, test_labels);
  Metrics::printMetrics(test_labels, test_pred);
  delete[] test_pred;
  for (int i = 0; i < texts.getLength(); i++) {
    delete[] vectors[i];
  }
  delete[] vectors;
  delete[] camera_ids;
  delete model;
  return 0;
}
