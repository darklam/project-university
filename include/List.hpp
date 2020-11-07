#ifndef LIST_H
#define LIST_H

template <typename T>
class Node {
 public:
  Node(T* value) { this->value = value; }
  Node(T* value, Node<T>* previous) {
    this->value = value;
    this->previous = previous;
  }

  Node<T>** getNext() { return &(this->next); }

  void setNext(Node<T>* next) {
    this->next = next;
  }

  Node<T>** getPrevious() { return &(this->previous); }

  T* getValue() { return this->value; }

  void clear(bool isArray) {
    if (isArray) {
      delete[] this->value;
    } else {
      delete this->value;
    }
  }

 private:
  T* value;
  Node<T>* next = nullptr;
  Node<T>* previous = nullptr;
};

template <typename T>
class List {
 public:
  List() {}

  List(bool isArray) {
    this->isArray = true;
  }

  ~List() {
    Node<T>* current = this->root;
    while (current != nullptr) {
      auto next = *(current->getNext());
      current->clear(this->isArray);
      delete current;
      current = next;
    }
  }

  void add(T* value) {
    if (this->lastNode == nullptr) {
      Node<T>** current = &this->root;
      Node<T>* previous = nullptr;
      while (*current != nullptr) {
        previous = *current;
        current = (*current)->getNext();
      }
      *current = new Node<T>(value, previous);
      this->lastNode = *current;
    } else {
      this->lastNode->setNext(new Node<T>(value, this->lastNode));
      this->lastNode = *(this->lastNode->getNext());
    }
    this->length++;
  }

  T* get(int index) {
    auto current = &this->root;
    int currentIndex = 0;
    while (currentIndex != index) {
      current = (*current)->getNext();
      currentIndex++;
    }

    return (*current)->getValue();
  }

  int getLength() { return this->length; }

  Node<T>* getRoot() { return this->root; }

 private:
  Node<T>* root = nullptr;
  Node<T>* lastNode = nullptr;
  int length = 0;
  bool isArray = false;
};

#endif