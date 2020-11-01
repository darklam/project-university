#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "List.hpp"
#include "Utils.hpp"

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
  char* key;
  T value;
};

template <typename T>
class Item {
 public:
  Item(char* key, T* value) {
    this->key = key;
    this->value = value;
  }

  char* getKey() { return this->key; }

  T* getValue() { return this->value; }

  void setValue(T* value) { this->value = value; }

  void cleanUp(bool isArray) {
    if (isArray) {
      delete[] this->value;
    } else {
      delete this->value;
    }
  }

 private:
  char* key;
  T* value;
};

template <typename T>
class HashMap {
 public:
  HashMap(bool isArray) {
    this->buckets = new List<Item<T>>*[this->bucketSize];
    for (int i = 0; i < this->bucketSize; i++) {
      this->buckets[i] = nullptr;
    }
    this->isArray = isArray;
  }

  void set(char* key, T* value) {
    unsigned long index = this->hashFunc(key);
    if (index >= this->bucketSize) {
      printf("Wow this hash function sucks\n");
      exit(EXIT_FAILURE);
    }
    List<Item<T>>* bucket = this->buckets[index];
    if (bucket == nullptr) {
      this->buckets[index] = new List<Item<T>>();
      bucket = this->buckets[index];
    }
    for (Node<Item<T>>* cur = bucket->getRoot(); cur != nullptr;
         cur = *(cur->getNext())) {
      Item<T>* val = cur->getValue();
      if (Utils::compareStrings(val->getKey(), key)) {
        val->setValue(value);
        return;
      }
    }
    bucket->add(new Item<T>(key, value));
  }

  List<Entry<T>>* getEntries() {
    List<Entry<T>>* entries = new List<Entry<T>>();
    for (int i = 0; i < this->bucketSize; i++) {
      if (this->buckets[i] == nullptr) {
        continue;
      }

      List<Item<T>>* current = this->buckets[i];
      for (Node<Item<T>>* cur = current->getRoot(); cur != nullptr;
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

  HashResult<T>* get(char* key) {
    unsigned long index = this->hashFunc(key);
    if (index >= this->bucketSize) {
      printf("Wow this hash function sucks\n");
      exit(EXIT_FAILURE);
    }
    List<Item<T>>* bucket = this->buckets[index];
    if (bucket == nullptr) {
      return new HashResult<T>();
    }
    for (auto current = bucket->getRoot(); current != nullptr;
         current = *(current->getNext())) {
      auto value = current->getValue();
      if (Utils::compareStrings(key, value->getKey())) {
        auto val = value->getValue();
        return new HashResult<T>(val);
      }
    }
    return new HashResult<T>();
  }

  ~HashMap() {
    for (int i = 0; i < this->bucketSize; i++) {
      List<Item<T>>* current = this->buckets[i];
      if (current == nullptr) {
        continue;
      }
      Node<Item<T>>* cur = current->getRoot();
      while (cur != nullptr) {
        Item<T>* val = cur->getValue();
        val->cleanUp(this->isArray);
        cur = *(cur->getNext());
      }
      delete current;
    }
    delete[] this->buckets;
  }

 private:
  List<Item<T>>** buckets;
  unsigned long bucketSize = 200;
  unsigned long hashFunc(char* key) {
    int len = strlen(key);
    unsigned long sum = 0;
    for (int i = 0; i < len; i++) {
      sum += (int)key[i];
    }

    return sum % this->bucketSize;
  }
  bool isArray = false;
};

#endif