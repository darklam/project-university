#include "Pairs.hpp"
#include "CustomVector.hpp"
#include "Clique.hpp"
#include "CSV.hpp"
#include "List.hpp"
#include "HashMap.hpp"
#include "Set.hpp"
#include "string"

void Pairs::PairsToClique(CustomVector<Pair*>* pairs, Clique* clique) {
  for (auto i = 0; i < pairs->getLength(); i++) {
    auto pair = (*pairs)[i];
    if (pair->value == 0) {
      clique->NegativePair(pair->getId1(), pair->getId2());
    }else{
      clique->setPair(pair->getId1(), pair->getId2());
    }
    delete pair;
  }
  delete pairs;
}

void Pairs::deleteEntries(List<Entry<Set*>*>* entries){
  for(auto i = entries->getRoot(); i != nullptr; i = *(i->getNext())){
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
  delete entries;
}

List<Entry<Set*>*>* Pairs::RemoveDup(List<Entry<Set*>*>* entries) {
  HashMap<Set*> dedupe;
  for (auto j = entries->getRoot(); j != nullptr; j = *(j->getNext())) {
    auto val = j->getValue();
    auto item = val->value;
    std::string key = std::to_string((intptr_t)item);
    dedupe.set(key, item);
    delete val;
  }
  delete entries;
  return dedupe.getEntries();
}

std::string Pairs::sortIds(std::string id1, std::string id2){
  if(id1.compare(id2) >= 0){
    return id1 + "," + id2 + "," + "0";
  }else{
    return id2 + "," + id1 + "," + "0";
  }
}


HashMap<std::string>* Pairs::createDataset(List<Entry<Set*>*>* positive, Clique *clique){
  auto pairs = new HashMap<std::string>(50000);
  for (auto i = positive->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto cur = i->getValue();
    auto item = cur->value;
    auto items = item->getItems();
    for (auto j = items->getRoot(); j != nullptr; j = *(j->getNext())) {
      auto val = j->getValue();
      for (auto k = *(j->getNext()); k != nullptr; k = *(k->getNext())) {
        auto val1 = k->getValue();
        auto str = val->value + "," + val1->value + "," + "1";
        pairs->set(str, str);
      }
      delete val;
    }
    delete items;
  }
  for (auto i = positive->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto cur = i->getValue();
    auto item = cur->value;
    auto items = item->getItems();
    auto negatives = clique->getNegatives(items->getRoot()->getValue()->value);
    if(negatives->getLength() == 0){
      for (auto j = items->getRoot(); j != nullptr; j = *(j->getNext())) {
        delete j->getValue();
      }
      delete items;
      for (auto k = negatives->getRoot(); k != nullptr; k = *(k->getNext())) {
        auto neg = k->getValue();
        delete neg;
      }
      delete negatives;
      continue;
    }
    for (auto j = items->getRoot(); j != nullptr; j = *(j->getNext())) {
      auto val = j->getValue();
      for (auto k = negatives->getRoot(); k != nullptr; k = *(k->getNext())) {
        auto neg = k->getValue();
        auto str = sortIds(val->value, neg->value);
        pairs->set(str, str);
      }
      delete val;
      
    }
    for (auto k = negatives->getRoot(); k != nullptr; k = *(k->getNext())) {
      auto neg = k->getValue();
      delete neg;
    }
    delete negatives;
    delete items;
  }
  return pairs;
}

HashMap<std::string>* Pairs::createDataset(List<Entry<Set*>*>* positive, List<Entry<Set*>*>* negative){
  auto pairs = new HashMap<std::string>(50000);
  for (auto i = positive->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto cur = i->getValue();
    auto item = cur->value;
    auto items = item->getItems();
    for (auto j = items->getRoot(); j != nullptr; j = *(j->getNext())) {
      auto val = j->getValue();
      for (auto k = *(j->getNext()); k != nullptr; k = *(k->getNext())) {
        auto val1 = k->getValue();
        auto str = val->value + "," + val1->value + "," + "1";
        pairs->set(str, str);
      }
      delete val;
    }
    delete items;
  }

  for (auto i = negative->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto cur = i->getValue();
    auto item = cur->value;
    auto items = item->getItems();
    auto key = cur->key;
    for(auto j = items->getRoot(); j != nullptr; j = *(j->getNext())){
      auto val = j->getValue();
      auto str = sortIds(key, val->value);
      pairs->set(str, str);
      delete val;
    }
    delete items;
  }
  return pairs;
}


HashMap<std::string>* Pairs::PairsToDataset(CustomVector<Pair*>* pairs, std::string type, std::string output){
    auto clique = new Clique();
    PairsToClique(pairs, clique);
    auto positives = clique->getPositiveEntries();
    auto pos_unique = RemoveDup(positives);
    auto negatives = clique->getNegativeEntries();
    auto _pairs = createDataset(pos_unique, negatives);
    auto neg_unique = RemoveDup(negatives);
    // auto _pairs = createDataset(pos_unique, clique);
    if(type == "pairs"){
      CSV::WriteCSVPairs(output, pos_unique);
    }else if (type == "all"){
      CSV::WriteCSV(output, pos_unique);
    }
    
    deleteEntries(pos_unique);
    deleteEntries(neg_unique);
    delete clique;
    // return _pairs;
    return nullptr;
}