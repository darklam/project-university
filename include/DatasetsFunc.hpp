#ifndef DATASETSFUNC_HPP
#define DATASETSFUNC_HPP
#include "FastVector.hpp"
#include <string>

namespace DatasetsFunc {
    void balanceDataset(FastVector<std::string>& src, FastVector<std::string>& dest, int size);
    void RandomizeDataset(FastVector<std::string>& dataset);
    void train_test_split(FastVector<std::string>& dataset, FastVector<std::string>& train, FastVector<std::string>& test, FastVector<std::string>& validation);
    void split_in_batches(FastVector<int>& batches, int size);
}

#endif // !DATASETSFUNC_HPP
