/*
 * @Author: runwiththewind23 19945243142@163.com
 * @Date: 2024-08-08 12:19:59
 * @LastEditors: runwiththewind23 19945243142@163.com
 * @LastEditTime: 2024-08-08 12:53:12
 * @FilePath: /sxd/chat/src/lock/lock.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#pragma once
/*线程同步包装类*/
#include <exception>
#include <pthread.h>
#include <semaphore.h>
class sem {
private:
  sem_t m_sem;

public:
  sem();
  sem(int num);
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
  bool wait(pthread_mutex_t *m_mutex);
  bool timewait(pthread_mutex_t *m_mutex, struct timespec t);
  bool signal();
  bool broadcast();
};
