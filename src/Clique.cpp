#include "HashMap.hpp"
#include "Clique.hpp"
#include "List.hpp"
#include "Utils.hpp"
#include <iostream>
#include "Set.hpp"


Clique::Clique(){
    this->Map = new HashMap<Set*>();
}

Clique::~Clique(){
    delete this->Map;
}


void Clique::Pair(std::string id1, std::string id2){
    HashResult<Set*> res1;
    HashResult<Set*> res2;
    this->Map->get(id1, &res1);
    this->Map->get(id2, &res2);
    if (res1.hasValue && res2.hasValue) {
        if (res1.value != res2.value) {
            res1.value->merge(res2.value);
            auto entries = res2.value->getItems();
            for (auto it = entries->getRoot(); it != nullptr; it = *(it->getNext())) {
                auto v = it->getValue();
                auto actual = v->value;
                HashResult<Set*> res3;
                this->Map->get(actual, &res3);
                if (res3.hasValue) {
                    this->Map->set(actual, res1.value);
                }
                delete v;
            }
            delete entries;
            delete res2.value;
            this->Map->set(id2, res1.value);
        }
        res1.value->add(id2);
    } else if (res1.hasValue && !res2.hasValue) {
        res1.value->add(id2);
        this->Map->set(id2, res1.value);
    } else if (res2.hasValue && !res1.hasValue) {
        res2.value->add(id1);
    } else {
        Set* set = new Set();
        set->add(id1);
        set->add(id2);
        this->Map->set(id1, set);
        this->Map->set(id2, set);
    }
}

List<Entry<Set*>*>* Clique::getEntries(){
    return this->Map->getEntries();
}
