#ifndef CLIQUE_HPP
#define CLIQUE_HPP
#include "HashMap.hpp"
#include "List.hpp"

class Clique{
    public:
        Clique();
        ~Clique();
        void Add(char *);
        void Pair(char *, char *);
        List<Entry<List<char*>*>*>* getEntries();
    private:
        HashMap<List<char *>*>* Map;
        bool ExistsInList(List<char *>* , char *);
        void ConcatLists(List<char *>*, List<char *>*);
        void UpdateLists(List<char *>*, List<char *>*);
};


#endif