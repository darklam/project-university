#ifndef JOB_SCHEDULER_HPP
#define JOB_SCHEDULER_HPP
#include <functional>
#include <thread>
#include <mutex>
#include <semaphore.h>
#include <iostream>
#include "Queue.hpp"

struct Job {
  std::function<void()> job;

  Job(std::function<void()> job) {
    this->job = job;
  }

  void run() {
    job();
  }
};

class JobScheduler {
public:

  JobScheduler() {
    queue = new Queue<Job*>(queueMax);
    int code = sem_init(&empty, 0, 0);
    if (code != 0) {
      perror("Semaphore error: ");
      exit(EXIT_FAILURE);
    }

    code = sem_init(&full, 0, queueMax);
    if (code != 0) {
      perror("Semaphore error: ");
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < cores; i++) {
      threadPool[i] = new std::thread([this] {
        while (true) {
          sem_wait(&this->empty);
          QueueResult<Job*> res;
          this->m.lock();
          this->queue->pop(res);
          this->m.unlock();
          sem_post(&this->full);
          if (!res.hasValue) {
            std::cout << "What the hell bruv?\n";
          }
          res.item->run();

          delete res.item;
        }
      });
    }
  }

  void addJob(Job* j) {
    sem_wait(&full);
    m.lock();
    queue->add(j);
    m.unlock();
    sem_post(&empty);
    // int emptyValue, fullValue;
    // sem_getvalue(&empty, &emptyValue);
    // sem_getvalue(&full, &fullValue);
    // std::cout << "Added an item. Full: " << fullValue << " Empty: " << emptyValue << std::endl;
  }

  ~JobScheduler() {
    delete queue;
    delete[] threadPool;
  }

private:
  int cores = std::thread::hardware_concurrency();
  int queueMax = cores * 2; // * 2 to have some jobs on the ready
  Queue<Job*>* queue;
  int jobCount = 0;
  sem_t empty;
  sem_t full;
  std::mutex m;
  std::thread** threadPool = new std::thread*[cores];
};

#endif