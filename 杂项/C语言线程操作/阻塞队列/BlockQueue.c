#include "BlockQueue.h"
// 用数组实现的
BlockQ *blockq_create(void){
  BlockQ * BlockQueue = malloc(sizeof(BlockQ));

  BlockQueue->front = 0;
  BlockQueue->size = 0;
  BlockQueue->rear = 0;
  pthread_mutex_init(&BlockQueue->mutex,NULL);
  pthread_cond_init(&BlockQueue->not_empty,NULL);
  pthread_cond_init(&BlockQueue->not_full,NULL);

  return BlockQueue;
}


void blockq_destroy(BlockQ *q){
  if (!q) {
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_full);
    pthread_cond_destroy(&q->not_empty);
    free(q);
  }
}

bool blockq_empty(BlockQ *q){
  return q->size == 0;
}
bool blockq_full(BlockQ *q){
  return q->size == N;
}
void blockq_push(BlockQ *q, E val){
  pthread_mutex_lock(&q->mutex);
  while (blockq_full(q)) {  // 满了就会阻塞，解除阻塞再次检查以避免虚假唤醒
    pthread_cond_wait(&q->not_full, &q->mutex); // 阻塞
  } // 未满
  q->elements[q->rear] = val;
  q->rear = (q->rear + 1) % N;
  q->size++;
  pthread_cond_broadcast(&q->not_empty);  // 唤醒多个线程
  pthread_mutex_unlock(&q->mutex);
}
E blockq_pop(BlockQ *q){
  pthread_mutex_lock(&q->mutex);
  while (blockq_empty(q)){  // 空的就会阻塞，直到被唤醒
    pthread_cond_wait(&q->not_empty,&q->mutex); // 阻塞
  }// 不是空的
  E data = q->elements[q->front];
  q->front = (q->front + 1) % N;
  q->size--;
  pthread_cond_broadcast(&q->not_full); // 唤醒多个线程
  pthread_mutex_unlock(&q->mutex);
  return data;
}
E blockq_peek(BlockQ *q){
  pthread_mutex_lock(&q->mutex);
  E data = q->elements[q->front];
  pthread_mutex_unlock(&q->mutex);
  return data;
}