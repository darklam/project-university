#include "HashMap.hpp"
#include "Clique.hpp"
#include "List.hpp"
#include "Utils.hpp"
#include <iostream>
#include "Set.hpp"


Clique::Clique(){
    this->Positive = new HashMap<Set*>();
    this->Negative = new HashMap<Set*>();
}

Clique::~Clique(){
    delete this->Positive;
    delete this->Negative;
}


void Clique::setPair(std::string id1, std::string id2){
    if(this->existInPositives(id1, id2)){
        return;
    }
    if(this->existInNegatives(id1, id2)){
        return;
    }
    int flag = this->PairMergeNegatives(id1, id2);
    
    HashResult<Set*> neg;
    this->Negative->get(id1, &neg);
    if(!neg.hasValue){
        printf("yep shit happens\n");
        exit(1);
    }
    HashResult<Set*> neg1;
    this->Negative->get(id2, &neg1);
    if(!neg1.hasValue){
        printf("yep shit happens\n");
        exit(1);
    }
    HashResult<Set*> pos1;
    HashResult<Set*> pos2;
    this->Positive->get(id1, &pos1);
    this->Positive->get(id2, &pos2);
    HashMap<Set*> dedupe;
    if (pos1.hasValue && pos2.hasValue) {
        if (pos1.value != pos2.value) {
            pos1.value->merge(pos2.value);
            auto entries = pos2.value->getItems();
            for (auto it = entries->getRoot(); it != nullptr; it = *(it->getNext())) {
                auto v = it->getValue();
                auto actual = v->value;
                HashResult<Set*> res3;
                this->Positive->get(actual, &res3);
                if (res3.hasValue) {
                    this->Positive->set(actual, pos1.value);
                }
                HashResult<Set*> not1;
                this->Negative->get(actual, &not1);
                if(not1.hasValue){
                    if(not1.value != neg1.value){
                        std::string key = std::to_string((intptr_t)not1.value);
                        dedupe.set(key, not1.value);
                    }
                }
                this->Negative->set(actual, neg.value);
                delete v;
            }
            delete entries;
            delete pos2.value;
            this->Positive->set(id2, pos1.value);
            if(flag == 1){
                delete neg1.value;
                this->Negative->set(id2, neg.value);
            }
        }
        pos1.value->add(id2);
        auto ee = dedupe.getEntries();
        for(auto i = ee->getRoot(); i != nullptr; i = *(i->getNext())){
            auto cur = i->getValue();
            auto item = cur->value;
            auto items = item->getItems();
            for (auto j = items->getRoot(); j != nullptr; j = *(j->getNext())) {
                auto val = j->getValue();
                delete val;
            }
            delete items;
            delete item;
            delete cur;
        }
        delete ee;
    } else if (pos1.hasValue && !pos2.hasValue) {
        pos1.value->add(id2);
        this->Positive->set(id2, pos1.value);
    } else if (pos2.hasValue && !pos1.hasValue) {
        pos2.value->add(id1);
        this->Positive->set(id1, pos2.value);
    } else {
        Set* set = new Set();
        set->add(id1);
        set->add(id2);
        this->Positive->set(id1, set);
        this->Positive->set(id2, set);
    }
    this->UpdateNegativesWithNewPair(id1);
    
}

int Clique::PairMergeNegatives(std::string id1, std::string id2){
    HashResult<Set*> neg1;
    HashResult<Set*> neg2;
    this->Negative->get(id1, &neg1);
    this->Negative->get(id2, &neg2);
    if (neg1.hasValue && neg2.hasValue) {
        if (neg1.value != neg2.value) {
            neg1.value->merge(neg2.value);
            return 1;
        }
    } else if (neg1.hasValue && !neg2.hasValue) {
        this->Negative->set(id2, neg1.value);
    } else if (neg2.hasValue && !neg1.hasValue) {
        this->Negative->set(id1, neg2.value);
    }else{
        Set* set = new Set();
        this->Negative->set(id1, set);
        this->Negative->set(id2,set);
    }
    return 0;
}

void Clique::UpdateNegativesWithNewPair(std::string id1){
    HashResult<Set*> pos1;
    this->Positive->get(id1, &pos1);
    if(!pos1.hasValue){
        exit(1);
    }
    HashResult<Set*> neg;
    this->Negative->get(id1, &neg);
    if(!neg.hasValue){
        return;
    }
    auto entries = neg.value->getItems();
    for (auto it = entries->getRoot(); it != nullptr; it = *(it->getNext())) {
        auto v = it->getValue();
        auto actual = v->value;
        HashResult<Set*> res3;
        this->Negative->get(actual, &res3);
        if (res3.hasValue) {
            res3.value->merge(pos1.value);
        }else{
            exit(1);
        }
        delete v;
    }
    delete entries;
}

void Clique::NegativePair(std::string id1, std::string id2){
    if(this->existInNegatives(id1, id2)){
        return;
    }
    if(this->existInPositives(id1, id2)){
        return;
    }
    HashResult<Set*> pos1;
    HashResult<Set*> pos2;
    this->Positive->get(id1, &pos1);
    this->Positive->get(id2, &pos2);
    HashResult<Set*> neg1;
    HashResult<Set*> neg2;
    this->Negative->get(id1, &neg1);
    this->Negative->get(id2, &neg2);
    if(!neg1.hasValue){
        if(pos2.hasValue){
            Set* set = new Set();
            set->merge(pos2.value);
            this->Negative->set(id1, set);
        }else{
            Set* set = new Set();
            set->add(id2);
            this->Negative->set(id1, set);
        }
    }else{
        
        if(pos2.hasValue){
            neg1.value->merge(pos2.value);
        }else{
            neg1.value->add(id2);
        }
    }

    if(!neg2.hasValue){
        if(pos1.hasValue){
            Set* set = new Set();
            set->merge(pos1.value);
            this->Negative->set(id2, set);
        }else{
            Set* set = new Set();
            set->add(id1);
            this->Negative->set(id2, set);
        }
    }else{
        
        if(pos1.hasValue){
            neg2.value->merge(pos1.value);
        }else{
            neg2.value->add(id1);
        }
    }
}



List<Entry<Set*>*>* Clique::getPositiveEntries(){
    return this->Positive->getEntries();
}

List<Entry<Set*>*>* Clique::getNegativeEntries(){
    return this->Negative->getEntries();
}

bool Clique::existInPositives(std::string id1, std::string id2){
    HashResult<Set*> pos1;
    HashResult<Set*> pos2;
    this->Positive->get(id1, &pos1);
    this->Positive->get(id2, &pos2);
    if(pos1.hasValue){
        if(pos1.value->exists(id2)){
            return true;
        }
        return false;
    }
    if(pos2.hasValue){
        if(pos2.value->exists(id1)){
            return true;
        }
        return false;
    }
    return false;
}


bool Clique::existInNegatives(std::string id1, std::string id2){
    HashResult<Set*> neg1;
    HashResult<Set*> neg2;
    this->Negative->get(id1, &neg1);
    this->Negative->get(id2, &neg2);

    if(neg1.hasValue){
        if(neg1.value->exists(id2)){
            return true;
        }
        return false;
    }
    if(neg2.hasValue){
        if(neg2.value->exists(id1)){
            return true;
        }
        return false;
    }
    return false;
}

List<Entry<std::string>*>* Clique::getNegatives(std::string id){
    HashResult<Set*> neg1;
    this->Negative->get(id, &neg1);

    if(neg1.hasValue){
        return neg1.value->getItems();
    }else{
        return nullptr;
    }
}