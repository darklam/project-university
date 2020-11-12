#include "CSV.hpp"
#include "Utils.hpp"
#include "FileSystem.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include "Clique.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include "Set.hpp"

void Pair::setIds(std::string id1, std::string id2){
    this->id1 = id1;
    this->id2 = id2;
}

std::string Pair::getId1(){
    return this->id1;
}

std::string Pair::getId2(){
    return this->id2;
}

void Pair::setValue(int val){
    this->value = val;
}

int Pair::getMatch(){
    return this->value;
}


CustomVector<Pair*>* CSV::ReadCSV(const std::string& path){
    // std::ifstream file(path);
    // std::string line;
    auto pairs = new CustomVector<Pair*>(10000);
    char *line = NULL;
    size_t len = 0;
    FILE *file = fopen(path.c_str(), "r");
    if (!file){
        printf("Shit just got real, file: %s\n", path.c_str());
        exit(EXIT_FAILURE);
    }
    int isFisrtLine = true;

    bool finished = false;

    while (!finished) {
        int length = getline(&line, &len, file);
        if (length == -1) {
            finished = true;
            continue;
        }
        if(isFisrtLine){
            isFisrtLine = false;
            continue;
        }
        auto pair = new Pair();
        std::string tokens[3];
        Utils::splitStringLite(std::string(line), ";", tokens, 3);
        if (tokens[0].length() == 0 || tokens[1].length() == 0) {
            printf("Something really bad happened\n");
        }
        pair->setValue(atoi(tokens[2].c_str()));
        pair->setIds(tokens[0],tokens[1]);
        pairs->add(pair);
    }
    if(line != NULL){
        delete(line);
    }
    fclose(file);

    return pairs;
}

void CSV::WriteCSVPairs(std::string path, List<Entry<Set*>*>* entries){
    FILE *file;
    file = fopen(path.c_str(), "w+");
    
    for (auto i = entries->getRoot(); i != nullptr; i = *(i->getNext())) {
        auto cur = i->getValue();
        auto item = cur->value;
        auto items = item->getItems();
        for (auto j = items->getRoot(); j != nullptr; j = *(j->getNext())) {
            auto val = j->getValue();
            for (auto k = *(j->getNext()); k != nullptr; k = *(k->getNext())) {
                auto val1 = k->getValue();
                // myfile << val->value << ", " << val1->value << std::endl;
                fprintf(file, "%s, %s\n", val->value.c_str(), val1->value.c_str());
            }
            delete val;
        }
        delete items;
        delete item;
        delete cur;
    }
    delete entries;
    // myfile.close();
    fclose(file);
}


void CSV::WriteCSV(std::string path, List<Entry<Set*>*>* entries){
    // std::ofstream myfile;
    // myfile.open(path);
    FILE *file;
    file = fopen(path.c_str(), "w+");
    for (auto i = entries->getRoot(); i != nullptr; i = *(i->getNext())) {
        auto cur = i->getValue();
        auto item = cur->value;
        auto items = item->getItems();
        bool flag = true;
        for (auto j = items->getRoot(); j != nullptr; j = *(j->getNext())) {
            auto val = j->getValue();
            if(flag){
                flag = false;
                fprintf(file, "%s", val->value.c_str());
            }else{
                fprintf(file, ", %s", val->value.c_str());
            }
            delete val;
        }
        fprintf(file, "\n");
        delete items;
        delete item;
        delete cur;
    }
    delete entries;
    fclose(file);
}

