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
    std::ifstream file(path);
    std::string line;
    auto pairs = new CustomVector<Pair*>(10000);
    if (file.bad()) {
        printf("Welp something was not okie dokie, file: %s\n", path.c_str());
        exit(EXIT_FAILURE);
    }
    int isFisrtLine = true;
    while (std::getline(file, line)) {
        if(isFisrtLine){
            isFisrtLine = false;
            continue;
        }
        auto pair = new Pair();
        // auto line_pair = Utils::splitString(std::string(line), ",");
        // auto id1 = line_pair->get(0);
        // auto id2 = line_pair->get(1);
        // int matches = atoi(line_pair->get(2).c_str());
        std::string tokens[3];
        Utils::splitStringLite(line, ";", tokens, 3);
        //delete line_pair;
        if (tokens[0].length() == 0 || tokens[1].length() == 0) {
            printf("Something really bad happened\n");
        }
        pair->setValue(atoi(tokens[2].c_str()));
        pair->setIds(tokens[0],tokens[1]);
        pairs->add(pair);
    }
    file.close();

    return pairs;
}

void CSV::WriteCSVPairs(std::string path, List<Entry<Set*>*>* entries){
    std::ofstream myfile;
    myfile.open(path);
    
    for (auto i = entries->getRoot(); i != nullptr; i = *(i->getNext())) {
        auto cur = i->getValue();
        auto item = cur->value;
        auto items = item->getItems();
        for (auto j = items->getRoot(); j != nullptr; j = *(j->getNext())) {
            auto val = j->getValue();
            for (auto k = *(j->getNext()); k != nullptr; k = *(k->getNext())) {
                auto val1 = k->getValue();
                myfile << val->value << ", " << val1->value << std::endl;
            }
            delete val;
        }
        delete cur;
    }
    delete entries;
    myfile.close();
}


void CSV::WriteCSV(std::string path, List<Entry<Set*>*>* entries){
    std::ofstream myfile;
    myfile.open(path);
    
    for (auto i = entries->getRoot(); i != nullptr; i = *(i->getNext())) {
        auto cur = i->getValue();
        auto item = cur->value;
        auto items = item->getItems();
        bool flag = true;
        for (auto j = items->getRoot(); j != nullptr; j = *(j->getNext())) {
            auto val = j->getValue();
            if(flag){
                flag = false;
                myfile << val->value;
            }else{
                myfile << ", " << val->value;
            }
            delete val;
        }
        myfile << std::endl;
        delete cur;
    }
    delete entries;
    myfile.close();
}

