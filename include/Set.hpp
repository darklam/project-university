#ifndef SET_H
#define SET_H

#include "HashMap.hpp"

class Set {
 public:
  Set();
  ~Set();
  void add(const std::string& item);
  bool exists(const std::string& item);
  List<Entry<std::string>*>* getItems();
  void merge(Set* set);

 private:
  HashMap<std::string>* items;
};

#endif