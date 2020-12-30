#ifndef CUSTOM_VECTOR_H
#define CUSTOM_VECTOR_H
#include "List.hpp"
#include <iostream>

template <typename T>
class CustomVector {
 public:
  CustomVector(int chunkSize) {
    this->chunkSize = chunkSize;
    this->chunks = new List<T*>();
    this->lastUsed = new T[this->chunkSize];
    this->lastUsedPlace = 0;
    this->chunks->add(this->lastUsed);
  }

  CustomVector() { 
    this->chunks = new List<T*>();
    this->lastUsed = new T[this->chunkSize];
    this->lastUsedPlace = 0;
    this->chunks->add(this->lastUsed);
  }

  void add(T item) {
    int place = this->length++;
    int chunk = place / this->chunkSize;
    int placeInChunk = place % this->chunkSize;
    T* current = nullptr;
    if (this->chunks->getLength() <= chunk) {
      current = new T[this->chunkSize];
      this->chunks->add(current);
    }
    if (current == nullptr && this->lastUsedPlace == chunk) {
      current = this->lastUsed;
    }
    if (current == nullptr) {
      current = this->chunks->get(chunk);
    }
    current[placeInChunk] = item;
    this->lastUsed = current;
    this->lastUsedPlace = chunk;
  }

  T get(int index) {
    int chunk = index / this->chunkSize;
    int placeInChunk = index % this->chunkSize;
    if (this->chunks->getLength() <= chunk) {
      std::cout << "What are you doing step bro?\n";
      exit(EXIT_FAILURE);
    }
    if (chunk == this->lastUsedPlace) {
      return this->lastUsed[placeInChunk];
    }
    T* current = this->chunks->get(chunk);
    return current[placeInChunk];
  }

  void set(int index, T value){
    int chunk = index / this->chunkSize;
    int placeInChunk = index % this->chunkSize;
    if (this->chunks->getLength() <= chunk) {
      std::cout << "What are you doing step bro?\n";
      exit(EXIT_FAILURE);
    }
    if (chunk == this->lastUsedPlace) {
      this->lastUsed[placeInChunk] = value;
    }
    T* current = this->chunks->get(chunk);
    current[placeInChunk] = value;
  }

  T operator[](int index) { return this->get(index); }

  int getLength() { return this->length; }

  ~CustomVector() {
    for (Node<T*>* i = this->chunks->getRoot(); i != nullptr;
         i = *(i->getNext())) {
      T* val = i->getValue();
      delete[] val;
    }
    delete this->chunks;
  }

 private:
  int chunkSize = 512;
  int length = 0;
  List<T*>* chunks;
  T* lastUsed = nullptr;
  int lastUsedPlace = -1;
};

#endif