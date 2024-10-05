#include "StrongestSafeQueue.h"
/******************* StrongestSafeQueue Test *********************/

#include <iostream>
#include "StrongestSafeQueue.h"
#include <thread>

void producer(StrongestSafeQueue<int>& queue) {
  for (int i = 0; i < 10; ++i) {
    std::cout << "Producing: " << i << std::endl;
    queue.push(i);
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 模拟生产过程的延迟
  }
}

void consumer(StrongestSafeQueue<int>& queue) {
  for (int i = 0; i < 10; ++i) {
    auto item = queue.try_pop();
    if (item) {
      std::cout << "Consuming: " << *item << std::endl;
    } else {
      std::cout << "Queue is empty, consumer is waiting..." << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 等待生产者生产
      --i; // 保证消费10个元素
    }
  }
}

int main() {
  StrongestSafeQueue<int> queue;

  // 创建生产者和消费者线程
  std::thread producer_thread(producer, std::ref(queue));
  std::thread consumer_thread(consumer, std::ref(queue));

  // 等待生产者和消费者线程结束
  producer_thread.join();
  consumer_thread.join();

  return 0;
}

/*

Producing: 0
Queue is empty, consumer is waiting...
Producing: 1
Consuming: 0
Consuming: 1
Queue is empty, consumer is waiting...
Producing: 2
Consuming: 2
Queue is empty, consumer is waiting...
Producing: 3
Consuming: 3
Queue is empty, consumer is waiting...
Producing: 4
Consuming: 4
Queue is empty, consumer is waiting...
Producing: 5
Consuming: 5
Queue is empty, consumer is waiting...
Producing: 6
Consuming: 6
Queue is empty, consumer is waiting...
Producing: 7
Consuming: 7
Queue is empty, consumer is waiting...
Producing: 8
Consuming: 8
Queue is empty, consumer is waiting...
Producing: 9
Consuming: 9

*/