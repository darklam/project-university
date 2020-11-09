#include "HashMap.hpp"
#include "Clique.hpp"
#include "List.hpp"
#include "Utils.hpp"
#include <iostream>


Clique::Clique(){
    this->Map = new HashMap<List<std::string>*>();
    //this->Stack = new List<List<std::string>*>();
}

Clique::~Clique(){
    delete this->Map;
    //this->DeleteStack();
}

void Clique::Add(std::string id){
    auto entry = this->Map->get(id);
    if(!entry->hasValue){
        auto list = new List<std::string>();
        list->add(id);
        this->Map->set(id, list);
        //this->AddToStack(list);
    }
    delete entry;
}

void Clique::Pair(std::string id1, std::string id2){
    auto camera1 = this->Map->get(id1);
    auto camera2 = this->Map->get(id2);
    if(!camera1->hasValue || !camera2->hasValue){
        printf("Not okie dokie here \n");
        exit(EXIT_FAILURE);
    }
    if(!this->ExistsInList(camera1->value, id2)){
        camera1->value->add(id2);
    }
    this->ConcatLists(camera1->value, camera2->value);
    this->UpdateLists(camera2->value, camera1->value);
    *camera2->value = *camera1->value;
    delete camera1;
    delete camera2;
}

Llist Clique::getEntries(){
    return this->Map->getEntries();
}

bool Clique::ExistsInList(List<std::string>* list, std::string key){
    for (auto i = list->getRoot(); i != nullptr; i = *(i->getNext())) { 
        auto item = i->getValue();
        if(item.compare(key) == 0) return true;
    }
    return false;
}

void Clique::ConcatLists(List<std::string>* l1, List<std::string>* l2){
    for (auto i = l2->getRoot(); i != nullptr; i = *(i->getNext())) { 
        auto item = i->getValue();
        if(!this->ExistsInList(l1, item)){
            l1->add(item);
        }
    }
}

void Clique::UpdateLists(List<std::string>* l1, List<std::string>* l2){
    for (auto i = l1->getRoot(); i != nullptr; i = *(i->getNext())) { 
        auto entry = this->Map->get(i->getValue());
        if(!entry->hasValue){
            printf("Not okie dokie\n");
            exit(EXIT_FAILURE);
        }
        *entry->value = *l2;
        delete entry;
    }
}

void Clique::AddToStack(List<std::string>* value){
    this->Stack->add(value);
}

void Clique::DeleteStack(){
    for (auto i = this->Stack->getRoot(); i != nullptr; i = *(i->getNext())) { 
        auto list = i->getValue();
        delete list;
    }
    delete this->Stack;
}