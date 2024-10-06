/******************* StrongestSafeQueue Test *********************/

#include <iostream>
#include "StrongestSafeQueue.h"
#include <thread>

#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <unistd.h>
#include "StrongestSafeQueue.h"  // 确保包含你的队列实现

void producer(StrongestSafeQueue<int>& queue, int id, int items) {
  for (int i = 0; i < items; ++i) {
    int value = id * 100 + i;  // 为每个生产者生成不同的值
    queue.push(value);
    std::cout << "Producer " << id << " produced: " << value << std::endl;
  }
  sleep(2);
}

void consumer(StrongestSafeQueue<int>& queue, int id, int items) {
  for (int i = 0; i < items; ++i) {
    int value;
    queue.wait_pop(value);
    std::cout << "Consumer " << id << " consumed: " << value << std::endl;
  }
  sleep(1);
}

void test_strongest_safe_queue() {
  StrongestSafeQueue<int> queue;

  const int producer_count = 3; // 生产者数量
  const int consumer_count = 3;  // 消费者数量
  const int items_per_producer = 5; // 每个生产者生产的项数

  std::vector<std::thread> producers;
  std::vector<std::thread> consumers;

  // 启动生产者线程
  for (int i = 0; i < producer_count; ++i) {
    producers.emplace_back(producer, std::ref(queue), i, items_per_producer);
  }

  // 启动消费者线程
  for (int i = 0; i < consumer_count; ++i) {
    consumers.emplace_back(consumer, std::ref(queue), i, items_per_producer);
  }

  // 等待所有生产者完成
  for (auto& producer : producers) {
    producer.join();
  }

  // 等待所有消费者完成
  for (auto& consumer : consumers) {
    consumer.join();
  }

  // 检查队列是否为空
  assert(queue.empty());
  std::cout << "All tests passed. The queue is empty." << std::endl;
}

int main() {
  test_strongest_safe_queue();
  return 0;
}
