#ifndef LOGISTIC_HPP
#define LOGISTIC_HPP
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include "CustomVector.hpp"
#include "FastVector.hpp"
#include "HashMap.hpp"
#include "JobScheduler.hpp"
#include "Types.hpp"
#include "Utils.hpp"

template <typename T>
class Logistic {
 public:
  Logistic(int vocab_size) {
    this->b0 = 1.0;
    this->b1 = new float[vocab_size * 2];
    this->loss_history = new FastVector<float>(10);
    for (int i = 0; i < vocab_size * 2; i++) {
      this->b1[i] = 1.0;
    }
    this->size = vocab_size * 2;
  };

  ~Logistic() {
    delete[] this->b1;
    delete this->loss_history;
  }

  void fit(FastVector<std::string>& dataset,
           T** vectors,
           HashMap<int>& ids,
           float learning_rate,
           int epocs = 1) {
    this->learning_rate = learning_rate;
    for (int e = 0; e < epocs; e++) {
      float total_loss = 0.0;
      for (int i = 0; i < dataset.getLength(); i++) {
        FastVector<float> vec(this->size);
        auto row = dataset.get(i);
        int y_true = this->getVector(row, vectors, ids, vec);
        auto pred = this->make_pred(vec);
        auto _loss = this->cost_function(y_true, pred);
        this->update_weights(vec, y_true, pred);
        total_loss += _loss;
      }
      std::cout << "Epoch: " << e
                << " - Loss: " << total_loss / dataset.getLength() << std::endl;
      this->loss_history->append(total_loss / dataset.getLength());
    }
  }

  void fit(FastVector<std::string>& dataset,
           T** vectors,
           HashMap<int>& ids,
           FastVector<int>& batches,
           float learning_rate,
           int epocs = 1) {
    auto scheduler = JobScheduler::getInstance();
    this->learning_rate = learning_rate;
    for (int e = 0; e < epocs; e++) {
      float total_loss = 0.0;
      std::mutex predMutex, updateMutex, lossMutex;
      for (int i = 0; i < batches.getLength(); i++) {
        scheduler->addJob(new Job([&batches, this, &predMutex, &updateMutex, &lossMutex, &ids, &total_loss, i, &vectors, &dataset] {
          int batch_size = batches[i];
          float* ws = new float[this->size];
          for (int k = 0; k < this->size; k++) {
            ws[i] = 0.0;
          }
          float batch_loss = 0.0;
          float w0 = 0.0;
          for (int b = 0; b < batch_size; b++) {
            FastVector<float> vec(this->size);
            auto row = dataset.get((i + 1) * b);
            int y_true = this->getVector(row, vectors, ids, vec);
            predMutex.lock();
            auto pred = this->make_pred(vec);
            predMutex.lock();
            auto _loss = this->cost_function(y_true, pred);
            for (auto j = 0; j < this->size; j++) {
              auto value = ws[j] + (pred - y_true) * vec[j];
              ws[j] = value;
            }
            w0 += pred - y_true;
            batch_loss += _loss;
          }
          updateMutex.lock();
          this->update_weights(ws, w0, batch_size);
          updateMutex.unlock();
          lossMutex.lock();
          total_loss += (batch_loss / batch_size);
          lossMutex.unlock();
        }));

        // std::cout << "Epoch: " << e << " - Batch: " << i << " - Loss:" <<
        // batch_loss / batch_size << std::endl;
      }
      scheduler->waitAllJobs();
      std::cout << "Epoch: " << e
                << " - Loss:" << total_loss / batches.getLength() << std::endl;
      this->loss_history->append(total_loss / batches.getLength());
    }
  }

  int* predict(FastVector<std::string>& dataset,
               T** vectors,
               HashMap<int>& ids,
               FastVector<int>& target) {
    int* predictions = new int[dataset.getLength()];
    auto scheduler = JobScheduler::getInstance();
    for (int i = 0; i < dataset.getLength(); i++) {
      scheduler->addJob(
          new Job([&dataset, i, &vectors, &ids, &target, &predictions, this] {
            FastVector<float> vec(this->size);
            auto row = dataset.get(i);
            int y_true = this->getVector(row, vectors, ids, vec);
            target.set(i, y_true);
            auto pred = this->make_pred(vec);
            if (pred >= 0.5) {
              predictions[i] = 1;
            } else {
              predictions[i] = 0;
            }
          }));
    }
    scheduler->waitAllJobs();
    return predictions;
  }

  float prob(FastVector<T>& vec) { return this->make_pred(vec); }

  float prob(T* vec) { return this->make_pred(vec); }

  FastVector<float>* getLoss() { return this->loss_history; }

 private:
  float b0;
  int size;
  float* b1;
  FastVector<float>* loss_history;
  float learning_rate;

  int getVector(std::string row,
                T** vectors,
                HashMap<int>& ids,
                FastVector<float>& result) {
    std::string tokens[3];
    Utils::splitStringLite(std::string(row), ",", tokens, 3);
    HashResult<int> res1;
    ids.get(tokens[0], &res1);
    if (!res1.hasValue) {
      printf("something bad\n");
      exit(1);
    }
    HashResult<int> res2;
    ids.get(tokens[1], &res2);
    if (!res2.hasValue) {
      printf("something bad\n");
      exit(1);
    }
    auto v1 = vectors[res1.value];
    auto v2 = vectors[res2.value];
    for (int i = 0; i < this->size / 2; i++) {
      result.append(v1[i]);
    }
    for (int i = 0; i < this->size / 2; i++) {
      result.append(v2[i]);
    }
    return atoi(tokens[2].c_str());
  }

  void update_weights(FastVector<float>& x, int y_true, float pred) {
    this->b0 = this->b0 - this->learning_rate * (pred - y_true);
    for (int i = 0; i < this->size; i++) {
      this->b1[i] =
          this->b1[i] - this->learning_rate * (pred - y_true) * x.get(i);
    }
  }

  void update_weights(float* ws, float w0, int size) {
    this->b0 = this->b0 - this->learning_rate * (w0 / size);
    for (int i = 0; i < this->size; i++) {
      this->b1[i] = this->b1[i] + this->learning_rate * (ws[i] / size);
    }
  }

  float make_pred(FastVector<float>& x) {
    float p = this->b0;
    // float p = 0;
    for (int i = 0; i < this->size; i++) {
      p += this->b1[i] * x.get(i);
    }
    float sigmoid = 1 / (1 + exp(-p));
    return sigmoid;
  }

  float make_pred(float* x) {
    float p = this->b0;
    // float p = 0;
    for (int i = 0; i < this->size; i++) {
      p += this->b1[i] * x[i];
    }
    float sigmoid = 1 / (1 + exp(-p));
    return sigmoid;
  }

  float cost_function(int y, float pred) {
    return (-y * log(pred) - (1 - y) * log(1 - pred));
  }
};

#endif