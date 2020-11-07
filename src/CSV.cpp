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

Pair::Pair(){
    this->id1 = nullptr;
    this->id2 = nullptr;
}

Pair::~Pair(){
    if(this->id1 != nullptr){
        delete[] this->id1;
        delete[] this->id2;
    }
}

void Pair::setIds(char* id1, char* id2){
    this->id1 = id1;
    this->id2 = id2;
}

char* Pair::getId1(){
    return this->id1;
}

char* Pair::getId2(){
    return this->id2;
}

void Pair::setValue(int val){
    this->value = val;
}

int Pair::getMatch(){
    return this->value;
}


List<Pair*>* CSV::ReadCSV(const char *path){
    FILE* fp;
    char* line;
    size_t len = 0;
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Welp something was not okie dokie, errno: %d\n", errno);
        exit(EXIT_FAILURE);
    }
  
    bool finished = false;
    auto pairs = new List<Pair*>();
    int isFisrtLine = true;
    while(!finished){
        auto pair = new Pair();
        int length = getline(&line, &len, fp);
        if(length == -1){
            finished = true;
            continue;
        }   
        if(isFisrtLine){
            isFisrtLine = false;
            continue;
        }
        auto line_pair = Utils::splitString(line, ",");
        auto id1 = line_pair->get(0);
        auto id2 = line_pair->get(1);
        int matches = atoi(line_pair->get(2));
        if (id1 == nullptr || id2 == nullptr) {
            printf("Something really bad happened\n");
        }
        pair->setValue(matches);
        pair->setIds(id1,id2);
        pairs->add(pair);
        delete line_pair;
    }
    fclose(fp);
    if (line != NULL) {
        free(line);
    }
    return pairs;
}

bool CSV::ExistsInList(List<std::string>* list, std::string key){
    for (auto i = list->getRoot(); i != nullptr; i = *(i->getNext())) { 
        auto item = i->getValue();
        if(item.compare(key) == 0) return true;
    }
    return false;
}

List<std::string>* CSV::CreateFileList(Llist entries){
    auto FList = new List<std::string>();
    for (auto i = entries->getRoot(); i != nullptr; i = *(i->getNext())) { 
        auto entry = i->getValue();
        auto list = entry->value;
        if(list->getLength() < 2) continue;
        // CSV::SortList(list);
        std::string s1("");
        auto flag = true;
        for (auto j = list->getRoot(); j != nullptr; j = *(j->getNext())) { 
            if(flag){
                flag = false;
                s1 += j->getValue();
            }else{
                s1 += ", ";
                s1 += j->getValue();
            }
        }
        if(!CSV::ExistsInList(FList, s1)){
            FList->add(s1);
        }
        delete entry;
    }
    return FList;
}

void CSV::WriteCSV(Llist entries){
    auto path = "Output.csv";
    std::ofstream myfile;
    myfile.open(path);
    auto FList = CSV::CreateFileList(entries);
    for (auto i = FList->getRoot(); i != nullptr; i = *(i->getNext())) { 
        myfile << i->getValue() << "\n";
    }
    myfile.close();
    delete FList;
}


