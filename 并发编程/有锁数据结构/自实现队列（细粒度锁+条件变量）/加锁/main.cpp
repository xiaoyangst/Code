/******************* StrongestSafeQueue Test *********************/

#include <iostream>
#include "StrongestSafeQueue.h"
#include <thread>

void producer(StrongestSafeQueue<int>& queue, int start, int count) {
  for (int i = start; i < start + count; ++i) {
    queue.push(i);
    std::cout << "Produced: " << i << std::endl;
  }
}

void consumer(StrongestSafeQueue<int>& queue, int id) {
  while (true) {
    std::shared_ptr<int> data = queue.try_pop();
    if (data) {
      std::cout << "Consumer " << id << " consumed: " << *data << std::endl;
    } else {
      // 当队列为空时，退出循环
      break;
    }
  }
}

int main() {
  StrongestSafeQueue<int> queue;

  // 创建生产者线程，负责将数据插入队列
  std::thread producer1(producer, std::ref(queue), 0, 10);
  std::thread producer2(producer, std::ref(queue), 10, 10);

  // 等待生产者完成
  producer1.join();
  producer2.join();

  // 创建消费者线程，负责从队列中取数据
  std::thread consumer1(consumer, std::ref(queue), 1);
  std::thread consumer2(consumer, std::ref(queue), 2);

  // 等待消费者完成
  consumer1.join();
  consumer2.join();

  return 0;
}
