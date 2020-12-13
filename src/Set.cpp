#include "Set.hpp"

Set::Set() {
  this->empty = true;
  this->items = new HashMap<std::string>();
}

void Set::add(const std::string& item) {
  this->empty = false;
  HashResult<std::string> res;
  this->items->get(item, &res);
  if (res.hasValue) {
    return;
  }
  this->items->set(item, item);
}

bool Set::exists(const std::string& item) {
  if(this->empty){
    return false;
  }
  HashResult<std::string> res;
  this->items->get(item, &res);
  return res.hasValue;
}

List<Entry<std::string>*>* Set::getItems() {
  return this->items->getEntries();
}

void Set::merge(Set* set) {
  this->empty = false;
  auto entries = set->getItems();
  for (auto i = entries->getRoot(); i != nullptr; i = *(i->getNext())) {
    auto val = i->getValue();
    auto actual = val->value;
    if (!this->exists(actual)) {
      this->add(actual);
    }
    delete val;
  }
  delete entries;
}

bool Set::isEmpty(){
  return this->empty;
}

Set::~Set() {
  delete this->items;
}