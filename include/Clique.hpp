#ifndef CLIQUE_HPP
#define CLIQUE_HPP
#include "HashMap.hpp"
#include "List.hpp"
#include <string>
#include "CustomVector.hpp"
#include "Set.hpp"

class Clique{
    public:
        Clique();
        ~Clique();
        void setPair(std::string, std::string);
        List<Entry<Set*>*>* getPositiveEntries();
        List<Entry<Set*>*>* getNegativeEntries();
        void NegativePair(std::string, std::string);
        List<Entry<std::string>*>* getNegatives(std::string);
    private:
        HashMap<Set*>* Positive;
        HashMap<Set*>* Negative;
        bool existInPositives(std::string, std::string);
        bool existInNegatives(std::string, std::string);
        void UpdateNegativesWithNewPair(std::string);
        int PairMergeNegatives(std::string, std::string);
};


#endif