#ifndef QUEUE_HPP
#define QUEUE_HPP

template <typename T>
struct QueueResult {
  T item;
  bool hasValue = false;

  void set(T item) {
    this->item = item;
    this->hasValue = true;
  }
};

template <typename T>
class Queue {
public:
  Queue(int maxSize = 10) {
    this->maxSize = maxSize;
    this->queue = new T[maxSize];
  }

  void add(T item) {
    if (size != maxSize) {
      this->queue[maxSize - size++ - 1] = item;
    }
  }

  void pop(QueueResult<T>& res) {
    if (size != 0) {
      res.set(this->queue[maxSize - --size - 1]);
    }
  }

  ~Queue() {
    delete[] queue;
  }
private:
  int maxSize;
  T* queue;
  int size = 0;
};

#endif