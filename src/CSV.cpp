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
    FILE* fp;
    char* line;
    size_t len = 0;
    fp = fopen(path.c_str(), "r");
    if (fp == NULL) {
        printf("Welp something was not okie dokie, errno: %d\n", errno);
        exit(EXIT_FAILURE);
    }
  
    bool finished = false;
    auto pairs = new CustomVector<Pair*>(10000);
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
        auto line_pair = Utils::splitString(std::string(line), ",");
        auto id1 = line_pair->get(0);
        auto id2 = line_pair->get(1);
        int matches = atoi(line_pair->get(2).c_str());
        if (id1.length() == 0 || id2.length() == 0) {
            printf("Something really bad happened\n");
        }
        pair->setValue(matches);
        pair->setIds(id1,id2);
        pairs->add(pair);
    }
    fclose(fp);
    if (line != NULL) {
        free(line);
    }
    return pairs;
}

bool CSV::Exists(List<std::string>* list, std::string key){
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
        auto vec = entry->value;
        if(vec->getLength() < 2) continue;
        // CSV::SortList(list);
        std::string s1("");
        auto flag = true;
        for (auto j = vec->getRoot(); j != nullptr; j = *(j->getNext())) { 
            if(flag){
                flag = false;
                s1 += j->getValue();
            }else{
                s1 += ", ";
                s1 += j->getValue();
            }
        }
        if(!CSV::Exists(FList, s1)){
            FList->add(s1);
        }
        delete entry;
    }
    return FList;
}

void CSV::WriteCSV(std::string path, Llist entries){
    std::ofstream myfile;
    myfile.open(path);
    auto FList = CSV::CreateFileList(entries);
    for (auto i = FList->getRoot(); i != nullptr; i = *(i->getNext())) { 
        myfile << i->getValue() << "\n";
    }
    myfile.close();
    delete FList;
}


