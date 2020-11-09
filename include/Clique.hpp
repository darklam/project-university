#ifndef CLIQUE_HPP
#define CLIQUE_HPP
#include "HashMap.hpp"
#include "List.hpp"
#include <string>
#include "Set.hpp"


class Clique{
    public:
        Clique();
        ~Clique();
        void Pair(std::string, std::string);
        List<Entry<Set*>*>* getEntries();
    private:
        HashMap<Set*>* Map;
};


#endif