#ifndef FAST_VECTOR_H
#define FAST_VECTOR_H
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <cstdint>
#include <cstring>

template <typename T>
class FastVector {
 public:
  FastVector(int size) {
    if (size <= 0) {
      std::cout << "U mad bro? Given size: " << size << std::endl;
      exit(EXIT_FAILURE);
    } 
    this->size = size;
    this->initArray();
  }

  FastVector() {
    this->initArray();
  }

  void append(T item) {
    if (this->length >= this->size) {
      int newSize = this->size * 2;
      T* newArray = new T[newSize];
      std::memcpy(newArray, this->items, this->size * sizeof(T));
      newArray[this->length] = item;
      this->size = newSize;
      delete[] this->items;
      this->items = newArray;
    } else {
      this->items[this->length] = item;
    }
    this->length++;
  }

  T get(int index) {
    if (index >= this->length) {
      std::cout << "What is this index mate? --->" << index << " of length " << this->length << std::endl;
      exit(EXIT_FAILURE);
    }
    return this->items[index];
  }

  T operator[](int index) {
    return this->get(index);
  }

  ~FastVector() {
    delete[] this->items;
  }

  int getLength() {
    return this->length;
  }

 private:
  void initArray() { this->items = new T[this->size]; }
  T* items = nullptr;
  int length = 0;
  int size = 500;
};

#endif