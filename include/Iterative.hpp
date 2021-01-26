#ifndef ITERATIVE_HPP
#define ITERATIVE_HPP
#include "FastVector.hpp"
#include "HashMap.hpp"
#include <string>
#include "Clique.hpp"
#include "Logistic.hpp"

namespace Iterative {
    void train(Clique* clique, Logistic<float>* model, std::string *camera_ids, FastVector<std::string>& dataset,
                HashMap<int>& existing, HashMap<int>& ids, float** vectors, int total_cameras);
    bool exists(std::string str1, std::string str2, HashMap<int>& existing);
    void resolve(Clique* clique, std::string path);
    void asVector(std::string str1, std::string str2, float** vectors, HashMap<int>& ids, FastVector<float>& result);
}

#endif // !ITERATIVE_HPP
