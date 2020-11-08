#ifndef CLIQUE_HPP
#define CLIQUE_HPP
#include "HashMap.hpp"
#include "List.hpp"
#include <string>



typedef List<Entry<List<std::string>*>*>* Llist;
class Clique{
    public:
        Clique();
        ~Clique();
        void Add(std::string);
        void Pair(std::string, std::string);
        Llist getEntries();
    private:
        HashMap<List<std::string>*>* Map;
        List<List<std::string>*>* Stack;
        bool ExistsInList(List<std::string>* , std::string);
        void ConcatLists(List<std::string>*, List<std::string>*);
        void UpdateLists(List<std::string>*, List<std::string>*);
        void AddToStack(List<std::string>*);
        void DeleteStack();
};


#endif