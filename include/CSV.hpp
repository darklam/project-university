#ifndef CSV_HPP
#define CSV_HPP
#include "List.hpp"
#include <string>
#include "Clique.hpp"
#include <string>
#include "CustomVector.hpp"

struct Pair{
    std::string id1;
    std::string id2;
    int value;
    void setIds(std::string, std::string);
    std::string getId1();
    std::string getId2();
    void setValue(int);
    int getMatch();
};

namespace CSV {
    CustomVector<Pair*>* ReadCSV(const std::string& path);
    void WriteCSV(std::string, Llist);
    bool Exists(List<std::string>* , std::string);
    List<std::string>* CreateFileList(Llist);
}

#endif