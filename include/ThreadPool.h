#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <sys/syscall.h>
#include <thread>
#include <unistd.h>
#include <vector>
class ThreadPool {
private:
  std::vector<std::thread> threads_;            // 线程池中的线程。
  std::queue<std::function<void()>> taskqueue_; // 任务队列。
  std::mutex mutex_; // 任务队列同步的互斥锁。
  // std::unique_lock<std::mutex> lock(mutex_);
  std::condition_variable condition_; // 任务队列同步的条件变量。
  std::atomic_bool
      stop_; // 在析构函数中，把stop_的值设置为true，全部的线程将退出。
public:
  // 在构造函数中将启动threadnum个线程，
  ThreadPool(size_t threadnum);

  // 把任务添加到队列中。
  void addtask(std::function<void()> task);
  size_t size();
  void stop(); //停止线程
               // 在析构函数中将停止线程。
  ~ThreadPool();
};