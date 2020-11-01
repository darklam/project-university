#ifndef CSV_HPP
#define CSV_HPP
#include "List.hpp"

struct Pair{
    char* id1;
    char* id2;
    Pair();
    ~Pair();
    void setIds(char *, char*);
    char* getId1();
    char* getId2();
};

namespace CSV {
    List<Pair*>* loadPairs(const char* path);
}

#endif