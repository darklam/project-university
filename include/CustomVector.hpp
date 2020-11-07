#ifndef CUSTOM_VECTOR_H
#define CUSTOM_VECTOR_H
#include "List.hpp"

template <typename T>
class CustomVector {
 public:
  CustomVector(int chunkSize) {
    this->chunkSize = chunkSize;
    this->chunks = new List<T*>(true);
  }

  CustomVector() {}

  void add(T* item) {
    int place = this->length++;
    int chunk = place / this->chunkSize;
    int placeInChunk = place % this->chunkSize;
    if (this->chunks->getLength() <= chunk) {
      T** arr = new T*[this->chunkSize];
      for (int i = 0; i < this->chunkSize; i++) {
        arr[i] = nullptr;
      }
      this->chunks->add(arr);
    }
    T** current = this->chunks->get(chunk);
    current[placeInChunk] = item;
  }

  T* get(int index) {
    int chunk = index / this->chunkSize;
    int placeInChunk = index % this->chunkSize;
    if (this->chunks->getLength() <= chunk) {
      return nullptr;
    }
    T** current = this->chunks->get(chunk);
    return current[placeInChunk];
  }

  T* operator[](int index) {
    return this->get(index);
  }

  int getLength() {
    return this->length;
  }

  ~CustomVector() {
    for (Node<T*>* i = this->chunks->getRoot(); i != nullptr; i = *(i->getNext())) {
      T** current = i->getValue();
      for (int i = 0; i < this->chunkSize; i++) {
        if (current[i] == nullptr) {
          continue;
        }
        delete current[i];
      }
    }
    delete this->chunks;
  }

 private:
  int chunkSize = 512;
  int length = 0;
  List<T*>* chunks;
};

#endif