#include "CSV.hpp"
#include "Utils.hpp"
#include "FileSystem.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>

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


List<Pair*>* CSV::loadPairs(const char *path){
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
        if(matches == 0){
            continue;
        }
        pair->setIds(id1,id2);
        pairs->add(pair);
    }
    fclose(fp);
    if (line != NULL) {
        free(line);
    }

    return pairs;
}