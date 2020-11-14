#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "List.hpp"
#include "Utils.hpp"
#include <string>

template <typename T>
struct HashResult {
 public:
  HashResult(T value) {
    this->hasValue = true;
    this->value = value;
  }

  HashResult() { this->hasValue = false; }

  bool hasValue;
  T value;
};

template <typename T>
struct Entry {
  std::string key;
  T value;
};

template <typename T>
class Item {
 public:
  Item(std::string key, T value) {
    this->key = key;
    this->value = value;
  }

  std::string getKey() { return this->key; }

  T getValue() { return this->value; }

  void setValue(T value) { this->value = value; }

 private:
  std::string key;
  T value;
};

template <typename T>
class HashMap {
 public:
  HashMap() {
    this->buckets = new List<Item<T>*>*[this->bucketSize];
    for (int i = 0; i < this->bucketSize; i++) {
      this->buckets[i] = nullptr;
    }
  }

  HashMap(int bucketSize) {
    this->bucketSize = bucketSize;
    this->buckets = new List<Item<T>*>*[this->bucketSize];
    for (int i = 0; i < this->bucketSize; i++) {
      this->buckets[i] = nullptr;
    }
  }

  void set(const std::string& key, T value) {
    unsigned long index = this->hashFunc(key);
    if (index >= this->bucketSize) {
      printf("Wow this hash function sucks\n");
      exit(EXIT_FAILURE);
    }
    List<Item<T>*>* bucket = this->buckets[index];
    if (bucket == nullptr) {
      this->buckets[index] = new List<Item<T>*>();
      bucket = this->buckets[index];
    }
    for (Node<Item<T>*>* cur = bucket->getRoot(); cur != nullptr;
         cur = *(cur->getNext())) {
      Item<T>* val = cur->getValue();
      if (val->getKey() == key) {
        val->setValue(value);
        return;
      }
    }
    bucket->add(new Item<T>(key, value));
  }

  List<Entry<T>*>* getEntries() {
    List<Entry<T>*>* entries = new List<Entry<T>*>();
    for (int i = 0; i < this->bucketSize; i++) {
      if (this->buckets[i] == nullptr) {
        continue;
      }

      List<Item<T>*>* current = this->buckets[i];
      for (Node<Item<T>*>* cur = current->getRoot(); cur != nullptr;
           cur = *(cur->getNext())) {
        Entry<T>* entry = new Entry<T>();
        Item<T>* currentBucket = cur->getValue();
        entry->key = currentBucket->getKey();
        entry->value = currentBucket->getValue();
        entries->add(entry);
      }
    }

    return entries;
  }

  HashResult<T>* get(const std::string& key) {
    unsigned long index = this->hashFunc(key);
    if (index >= this->bucketSize) {
      printf("Wow this hash function sucks\n");
      exit(EXIT_FAILURE);
    }
    List<Item<T>*>* bucket = this->buckets[index];
    if (bucket == nullptr) {
      return new HashResult<T>();
    }
    for (auto current = bucket->getRoot(); current != nullptr;
         current = *(current->getNext())) {
      auto value = current->getValue();
      if (key == value->getKey()) {
        auto val = value->getValue();
        return new HashResult<T>(val);
      }
    }
    return new HashResult<T>();
  }

  void get(const std::string& key, HashResult<T>* result) {
    unsigned long index = this->hashFunc(key);
    if (index >= this->bucketSize) {
      printf("Wow this hash function sucks\n");
      exit(EXIT_FAILURE);
    }
    List<Item<T>*>* bucket = this->buckets[index];
    if (bucket == nullptr) {
      return;
    }
    for (auto current = bucket->getRoot(); current != nullptr;
         current = *(current->getNext())) {
      auto value = current->getValue();
      if (key == value->getKey()) {
        auto val = value->getValue();
        result->hasValue = true;
        result->value = val;
      }
    }
  }

  ~HashMap() {
    for (int i = 0; i < this->bucketSize; i++) {
      List<Item<T>*>* current = this->buckets[i];
      if (current == nullptr) {
        continue;
      }
      for (Node<Item<T>*>* i = current->getRoot(); i != nullptr; i = *(i->getNext())) {
        Item<T>* val = i->getValue();
        delete val;
      }
      delete current;
    }
    delete[] this->buckets;
  }

 private:
  List<Item<T>*>** buckets;
  unsigned long bucketSize = 200;
  unsigned long hashFunc(std::string key) {
    unsigned long hash = 5381;
    auto str = key.c_str();
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c;

    return hash % this->bucketSize;
  }
};

#endif