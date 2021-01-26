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

  Job(std::function<void()> job);

  void run();
};

class JobScheduler {
public:

  void addJob(Job* j);

  void waitAllJobs();

  static JobScheduler* getInstance();

  static void destroy();

private:
  static JobScheduler* instance;
  int cores = std::thread::hardware_concurrency();
  int* statuses;
  int queueMax = cores * 2; // * 2 to have some jobs on the ready
  Queue<Job*>* queue;
  int jobCount = 0;
  sem_t empty;
  sem_t full;
  sem_t allFinished;
  std::mutex statusesMutex;
  std::mutex m;
  std::thread** threadPool = new std::thread*[cores];
  JobScheduler();
  ~JobScheduler();
};

#endif