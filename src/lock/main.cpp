/*
 * @Author: runwiththewind23 19945243142@163.com
 * @Date: 2024-08-08 12:58:16
 * @LastEditors: runwiththewind23 19945243142@163.com
 * @LastEditTime: 2024-08-08 12:58:57
 * @FilePath: /sxd/chat/src/lock/main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置
 * 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "lock.h"
#include <exception>
#include <iostream>
#include <queue>
#include <thread>
#include <unistd.h>
void runProducerConsumer() {
  std::queue<int> buffer;
  const int maxBufferSize = 5;
  sem items(0);
  sem spaces(maxBufferSize);
  locker bufferLock;
  cond bufferCond;

  auto producer = [&]() {
    for (int i = 0; i < 10; ++i) {
      spaces.wait(); // Wait for space to be available
      bufferLock.lock();

      buffer.push(i); // Produce item
      std::cout << "Produced: " << i << std::endl;

      bufferLock.unlock();
      items.post(); // Signal that a new item is available

      sleep(1); // Simulate production time
    }
  };

  auto consumer = [&]() {
    for (int i = 0; i < 10; ++i) {
      items.wait(); // Wait for an item to be available
      bufferLock.lock();

      int item = buffer.front();
      buffer.pop(); // Consume item
      std::cout << "Consumed: " << item << std::endl;

      bufferLock.unlock();
      spaces.post(); // Signal that space is now available

      sleep(2); // Simulate consumption time
    }
  };

  std::thread producerThread(producer);
  std::thread consumerThread(consumer);

  producerThread.join();
  consumerThread.join();
}

int main() {
  runProducerConsumer();
  return 0;
}