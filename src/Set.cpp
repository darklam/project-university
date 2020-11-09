#include "Set.hpp"

Set::Set() {
  this->items = new HashMap<std::string>();
}

void Set::add(const std::string& item) {
  HashResult<std::string> res;
  this->items->get(item, &res);
  if (res.hasValue) {
    return;
  }
  this->items->set(item, item);
}

bool Set::exists(const std::string& item) {
  HashResult<std::string> res;
  this->items->get(item, &res);
  return res.hasValue;
}

List<Entry<std::string>*>* Set::getItems() {
  return this->items->getEntries();
}

void Set::merge(Set* set) {
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

Set::~Set() {
  delete this->items;
}