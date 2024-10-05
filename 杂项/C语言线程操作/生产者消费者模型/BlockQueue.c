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
  if (q) {
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_full);
    pthread_cond_destroy(&q->not_empty);
    free(q);
  }
}

bool blockq_empty(BlockQ *q){
  pthread_mutex_lock(&q->mutex);
  bool is_empty = (q->size == 0);
  pthread_mutex_unlock(&q->mutex);
  return is_empty;
}
bool blockq_full(BlockQ *q){
  pthread_mutex_lock(&q->mutex);
  bool is_full = (q->size == N);
  pthread_mutex_unlock(&q->mutex);
  return is_full;
}
void blockq_push(BlockQ *q, E val){
  pthread_mutex_lock(&q->mutex);
  while (q->size == N) {
    pthread_cond_wait(&q->not_full, &q->mutex); // 阻塞，直到有空闲空间才解除
  } // 未满
  q->elements[q->rear] = val;
  q->rear = (q->rear + 1) % N;
  q->size++;
  pthread_cond_signal(&q->not_empty);
  pthread_mutex_unlock(&q->mutex);
}
E blockq_pop(BlockQ *q){
  pthread_mutex_lock(&q->mutex);
  while (q->size == 0){
    pthread_cond_wait(&q->not_empty,&q->mutex); // 阻塞，直到有数据才解除
  }// 不是空的
  E data = q->elements[q->front];
  q->front = (q->front + 1) % N;
  q->size--;
  pthread_cond_signal(&q->not_full);
  pthread_mutex_unlock(&q->mutex);
  return data;
}
E blockq_peek(BlockQ *q){
  pthread_mutex_lock(&q->mutex);
  while (q->size == 0){
    pthread_cond_wait(&q->not_empty,&q->mutex);
  }
  E data = q->elements[q->front];
  pthread_mutex_unlock(&q->mutex);
  return data;
}