#include <errno.h>
#include "public.h"
#include "BlockQueue.h"

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "BlockQueue.h"

#define PRODUCER_COUNT 3
#define CONSUMER_COUNT 3
#define TEST_ITERATIONS 5

// 生产者线程函数
void* producer(void* arg) {
  BlockQ* q = (BlockQ*)arg;
  for (int i = 0; i < TEST_ITERATIONS; ++i) {
    int val = rand() % 100;  // 随机生成 0-99 之间的整数
    blockq_push(q, val);
    printf("Producer [%ld] pushed: %d\n", pthread_self(), val);
    sleep(1);  // 模拟生产速度
  }
  return NULL;
}

// 消费者线程函数
void* consumer(void* arg) {
  BlockQ* q = (BlockQ*)arg;
  for (int i = 0; i < TEST_ITERATIONS; ++i) {
    int val = blockq_pop(q);
    printf("Consumer [%ld] popped: %d\n", pthread_self(), val);
    sleep(2);  // 模拟消费速度
  }
  return NULL;
}

int main() {
  srand(time(NULL));  // 随机种子

  BlockQ* queue = blockq_create();  // 创建阻塞队列

  pthread_t producers[PRODUCER_COUNT];
  pthread_t consumers[CONSUMER_COUNT];

  // 创建生产者线程
  for (int i = 0; i < PRODUCER_COUNT; ++i) {
    pthread_create(&producers[i], NULL, producer, (void*)queue);
  }

  // 创建消费者线程
  for (int i = 0; i < CONSUMER_COUNT; ++i) {
    pthread_create(&consumers[i], NULL, consumer, (void*)queue);
  }

  // 等待所有生产者线程结束
  for (int i = 0; i < PRODUCER_COUNT; ++i) {
    pthread_join(producers[i], NULL);
  }

  // 等待所有消费者线程结束
  for (int i = 0; i < CONSUMER_COUNT; ++i) {
    pthread_join(consumers[i], NULL);
  }

  blockq_destroy(queue);  // 销毁队列
  return 0;
}
