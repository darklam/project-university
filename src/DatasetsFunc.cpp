#include <iostream>
#include <string>
#include "DatasetsFunc.hpp"
#include "FastVector.hpp"
#include <cstdlib>
#define BATCH_SIZE 1024


void DatasetsFunc::balanceDataset(FastVector<std::string>& src,
                    FastVector<std::string>& dest,
                    int size) {
    int positives = 0;
    int negatives = 0;
    for (int i = 0; i < size; i++) {
        auto row = src.get(i);
        if (row.back() == '1') {
            dest.append(row);
            positives++;
        } else {
            if (positives >= negatives + 1) {
                dest.append(row);
                negatives++;
            }
        }
    }
}

void DatasetsFunc::RandomizeDataset(FastVector<std::string>& dataset) {
    int total_size = dataset.getLength();
    int pos1;
    int pos2;
    for (int i = 0; i < total_size; i++) {
        pos1 = rand() % total_size;
        pos2 = rand() % total_size;
        if (pos1 == pos2) {
            continue;
        }
        auto temp = dataset.get(pos1);
        dataset.set(pos1, dataset.get(pos2));
        dataset.set(pos2, temp);
    }
}

void DatasetsFunc::train_test_split(FastVector<std::string>& dataset,
                      FastVector<std::string>& train,
                      FastVector<std::string>& test,
                      FastVector<std::string>& validation) {
    RandomizeDataset(dataset);
    RandomizeDataset(dataset);
    int test_size = dataset.getLength() * 0.2 + 1;
    int validation_size = dataset.getLength() * 0.2 + 1;
    int train_size = dataset.getLength() * 0.6;
    for (int i = 0; i < test_size; i++) {
        test.append(dataset.get(i));
    }
    for (int i = test_size; i < test_size + validation_size; i++) {
        validation.append(dataset.get(i));
    }
    for (int i = test_size + validation_size; i < dataset.getLength(); i++) {
        train.append(dataset.get(i));
    }
}

void DatasetsFunc::split_in_batches(FastVector<int>& batches, int size){
    int b = size / BATCH_SIZE;
    int remaining = size % BATCH_SIZE;
    for (int i = 0; i < b; i++) {
        batches.append(BATCH_SIZE);
    }
    batches.append(remaining);
}