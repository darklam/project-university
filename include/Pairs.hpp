#ifndef PAIRS_HPP
#define PAIRS_HPP
#include "CustomVector.hpp"
#include "Clique.hpp"
#include "CSV.hpp"
#include "List.hpp"
#include "HashMap.hpp"
#include "Set.hpp"
#include "string"

namespace Pairs {
    void PairsToClique(CustomVector<Pair*>* pairs, Clique* clique);
    void deleteEntries(List<Entry<Set*>*>* entries);
    List<Entry<Set*>*>* RemoveDup(List<Entry<Set*>*>* entries);
    std::string sortIds(std::string id1, std::string id2);
    HashMap<std::string>* createDataset(List<Entry<Set*>*>* positive, Clique *clique);
    HashMap<std::string>* createDataset(List<Entry<Set*>*>* positive, List<Entry<Set*>*>* negative);
    HashMap<std::string>* PairsToDataset(CustomVector<Pair*>* pairs, std::string type, std::string output);
}


#endif // !1