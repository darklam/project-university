#ifndef CSV_HPP
#define CSV_HPP
#include "List.hpp"
#include <string>
#include "Clique.hpp"

struct Pair{
    char* id1;
    char* id2;
    int value;
    Pair();
    ~Pair();
    void setIds(char *, char*);
    char* getId1();
    char* getId2();
    void setValue(int);
    int getMatch();
};

namespace CSV {
    List<Pair*>* ReadCSV(const char* path);
    void WriteCSV(Llist);
    bool ExistsInList(List<std::string>* , std::string);
    List<std::string>* CreateFileList(Llist);
    // void SortList(List<char*>*);
}

#endif