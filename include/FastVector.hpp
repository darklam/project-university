#ifndef FAST_VECTOR_H
#define FAST_VECTOR_H
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <functional>

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
      for (int i = 0; i < this->length; i++) {
        newArray[i] = this->items[i]; // This was changed from memcpy cause it caused issues with std::string deleting the underlying pointer
      }
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

  bool includes(T item) {
    for (int i = 0; i < this->length; i++) {
      if (this->items[i] == item) {
        return true;
      }
    }
    return false;
  }

  void remove(int index) {
    if (index >= this->length) {
      return;
    }
    for (int i = index; i < this->length - 1; i++) {
      this->items[i] = this->items[i + 1];
    }
    this->length--;
  }

  FastVector<T>* filter(std::function<bool(T)> cond) {
    auto vec = new FastVector<T>(this->length);
    for (int i = 0; i < this->length; i++) {
      auto it = this->items[i];
      if (cond(it)) {
        vec->append(it);
      }
    }
    return vec;
  }

  void transform(std::function<T(T&)> func) {
    for (int i = 0; i < this->length; i++) {
      this->items[i] = func(this->items[i]);
    }
  }

  // This sets the length of the vector equal to the size (the reserved size)
  // with a value T
  void forceInit(T value) {
    this->length = this->size;
    for (int i = 0; i < this->size; i++) {
      this->items[i] = value;
    }
  }

  void set(int index, T value) {
    if (index >= this->size) {
      std::cout << "U w0t m8? Index: " << index << " Size: " << this->size << std::endl;
      exit(EXIT_FAILURE);
    }
    this->items[index] = value;
  }

 private:
  void initArray() { this->items = new T[this->size]; }
  T* items = nullptr;
  int length = 0;
  int size = 500;
};

#endif