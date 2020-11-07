#ifndef CLIQUE_HPP
#define CLIQUE_HPP
#include "HashMap.hpp"
#include "List.hpp"

typedef List<Entry<List<char*>*>*>* Llist;
class Clique{
    public:
        Clique();
        ~Clique();
        void Add(char *);
        void Pair(char *, char *);
        Llist getEntries();
    private:
        HashMap<List<char *>*>* Map;
        List<List<char*>*>* Stack;
        bool ExistsInList(List<char *>* , char *);
        void ConcatLists(List<char *>*, List<char *>*);
        void UpdateLists(List<char *>*, List<char *>*);
        void AddToStack(List<char*>*);
        void DeleteStack();
};


#endif