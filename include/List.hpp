#ifndef LIST_H
#define LIST_H

#include <cerrno>
#include <iostream>

template <typename T>
class Node {
 public:
  Node(T value) { this->value = value; }
  Node(T value, Node<T>* previous) {
    this->value = value;
    this->previous = previous;
  }

  Node<T>** getNext() { return &(this->next); }

  void setNext(Node<T>* next) { this->next = next; }

  Node<T>** getPrevious() { return &(this->previous); }

  void setPrevious(Node<T>* previous) { this->previous = previous; }

  T getValue() { return this->value; }

 private:
  T value;
  Node<T>* next = nullptr;
  Node<T>* previous = nullptr;
};

template <typename T>
class List {
 public:
  ~List() {
    Node<T>* current = this->root;
    while (current != nullptr) {
      auto next = *(current->getNext());
      delete current;
      current = next;
    }
  }

  void add(T value) {
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

  void remove(Node<T>* node) {
    for (Node<T>* i = this->root; i != nullptr; i = *(i->getNext())) {
      if (i == node) {
        Node<T>** previous = i->getPrevious();
        Node<T>** next = i->getNext();
        if (*previous != nullptr) {
          (*previous)->setNext(*next);
        }
        if (*next != nullptr) {
          (*next)->setPrevious(*previous);
        }

        if (i == this->root) {
          this->root = *next;
        }

        if (i == this->lastNode) {
          this->lastNode = *previous;
        }

        delete i;
        break;
      }
    }
  }

  T get(int index) {
    if (index >= this->length) {
      std::cout << "Invalid list access. Length " << this->length
                << ", index: " << index << std::endl;
      exit(EXIT_FAILURE);
    }
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
};

#endif