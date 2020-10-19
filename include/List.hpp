#ifndef LIST_H
#define LIST_H

template <typename T>
class Node {
 public:
  Node(T value) { this->value = value; }
  Node(T value, Node<T>* previous) {
    this->value = value;
    this->previous = previous;
  }

  Node<T>** getNext() { return &(this->next); }

  Node<T>** getPrevious() { return &(this->previous); }

  T getValue() { return this->value; }

 private:
  T value;
  Node<T>* next = nullptr;
  Node<T>* previous = nullptr;
};

template <typename T>
class List {
 public:
  List() {}

  ~List() {
    Node<T>* current = this->root;
    while (current != nullptr) {
      auto next = *(current->getNext());
      delete current;
      current = next;
    }
  }

  void add(T value) {
    Node<T>** current = &this->root;
    Node<T>* previous = nullptr;
    while (*current != nullptr) {
      previous = *current;
      current = (*current)->getNext();
    }
    *current = new Node<T>(value, previous);
    this->length++;
  }

  T* toArray() {
    T* arr = new T[this->length];
    Node<T>** current = &this->root;
    int index = 0;
    while (*current != nullptr) {
      arr[index] = (*current)->getValue();

      current = (*current)->getNext();
      index++;
    }
    return arr;
  }

  int getLength() { return this->length; }

 private:
  Node<T>* root = nullptr;
  int length = 0;
};

#endif