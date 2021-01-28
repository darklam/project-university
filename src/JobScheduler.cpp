#include "JobScheduler.hpp"
JobScheduler* JobScheduler::instance = nullptr;

JobScheduler* JobScheduler::getInstance() {
  if (JobScheduler::instance == nullptr) {
    JobScheduler::instance = new JobScheduler();
  }

  return JobScheduler::instance;
}

void JobScheduler::destroy() {
  if (JobScheduler::instance != nullptr) {
    delete JobScheduler::instance;
  }
}

JobScheduler::JobScheduler() {
  queue = new Queue<Job*>(queueMax);
  statuses = new int[cores];
  for (int i = 0; i < cores; i++) {
    statuses[i] = 0;
  }
  auto code = sem_init(&empty, 0, 0);
  if (code != 0) {
    perror("Semaphore error: ");
    exit(EXIT_FAILURE);
  }

  code = sem_init(&full, 0, queueMax);
  if (code != 0) {
    perror("Semaphore error: ");
    exit(EXIT_FAILURE);
  }

  code = sem_init(&allFinished, 0, 0);
  if (code != 0) {
    perror("Semaphore error: ");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < cores; i++) {
    threadPool[i] = new std::thread([this, i] {
      while (true) {
        sem_wait(&this->empty);
        this->statusesMutex.lock();
        this->statuses[i] = 1;
        this->statusesMutex.unlock();
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
        this->statusesMutex.lock();
        this->statuses[i] = 0;
        bool allDone = true;
        for (int k = 0; k < this->cores; k++) {
          if (this->statuses[k] != 0) {
            allDone = false;
            break;
          }
        }
        if (allDone && this->queue->getLength() == 0) {
          sem_post(&this->allFinished);
        }
        this->statusesMutex.unlock();
      }
    });
  }
}

void JobScheduler::addJob(Job* j) {
  sem_wait(&full);
  int value;
  sem_getvalue(&allFinished, &value);
  if (value == 1) {
    sem_wait(&allFinished);
  }
  m.lock();
  queue->add(j);
  m.unlock();
  sem_post(&empty);
  // int emptyValue, fullValue;
  // sem_getvalue(&empty, &emptyValue);
  // sem_getvalue(&full, &fullValue);
  // std::cout << "Added an item. Full: " << fullValue << " Empty: " <<
  // emptyValue << std::endl;
}

void JobScheduler::waitAllJobs() {
  sem_wait(&allFinished);
  return;
}

JobScheduler::~JobScheduler() {
  delete queue;
  for (int i = 0; i < cores; i++) {
    threadPool[i]->detach();
    delete threadPool[i];
  }
  delete[] threadPool;
  delete[] statuses;
  sem_destroy(&allFinished);
  sem_destroy(&full);
  sem_destroy(&empty);
}

Job::Job(std::function<void()> job) {
  this->job = job;
}

void Job::run() {
  job();
}