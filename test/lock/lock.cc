/*
 * @Author: runwiththewind23 19945243142@163.com
 * @Date: 2024-08-08 12:30:10
 * @LastEditors: runwiththewind23 19945243142@163.com
 * @LastEditTime: 2024-08-08 13:01:47
 * @FilePath: /sxd/chat/src/lock/lock.cc
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "lock.h"
/*#include <exception>
#include <pthread.h>
#include <semaphore.h>
class sem {
private:
  sem_t m_sem;

public:
  sem();
  sem(int sem);
  ~sem();
  bool wait();
  bool post();
};
class locker {
private:
  pthread_mutex_t m_mutex;

public:
  locker();
  ~locker();
  bool lock();
  bool unlock();
  pthread_mutex_t *get();
};
class cond {
private:
  pthread_cond_t m_cond;

public:
  cond();
  ~cond();
  bool wait();
  bool timewait();
  bool signal();
  bool broadcast();
};
*/
class SemaphoreException : public std::exception {
public:
  const char *what() const noexcept override {
    return "Semaphore initialization failed";
  }
};
sem::sem() {
  if (sem_init(&m_sem, 0, 0) != 0)
    throw SemaphoreException();
}
sem::sem(int num) {
  if (sem_init(&m_sem, 0, num) != 0)
    throw SemaphoreException();
}
sem::~sem() { sem_destroy(&m_sem); }
bool sem::wait() { return sem_wait(&m_sem) == 0; }
bool sem::post() { return sem_post(&m_sem) == 0; }

locker::locker() {
  if (pthread_mutex_init(&m_mutex, nullptr) != 0) {
    throw SemaphoreException();
  }
}
locker::~locker() { pthread_mutex_destroy(&m_mutex); }
bool locker::lock() { return pthread_mutex_lock(&m_mutex) == 0; }
bool locker::unlock() { return pthread_mutex_unlock(&m_mutex) == 0; }
pthread_mutex_t *locker::get() { return &m_mutex; }

cond::cond() {
  if (pthread_cond_init(&m_cond, nullptr) != 0)
    throw SemaphoreException();
}
cond::~cond() { pthread_cond_destroy(&m_cond); }
bool cond::wait(pthread_mutex_t *m_mutex) {
  return pthread_cond_wait(&m_cond, m_mutex) == 0;
}
bool cond::timewait(pthread_mutex_t *m_mutex, struct timespec t) {
  return pthread_cond_timedwait(&m_cond, m_mutex, &t) == 0;
}
bool cond::signal() { return pthread_cond_signal(&m_cond) == 0; }
bool cond::broadcast() { return pthread_cond_broadcast(&m_cond) == 0; }
