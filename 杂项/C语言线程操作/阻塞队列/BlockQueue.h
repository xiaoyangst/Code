#ifndef CWANG__BLOCKQUEUE_H_
#define CWANG__BLOCKQUEUE_H_

#include "public.h"
#define N 1024

typedef int E;

typedef struct {
  E elements[N];    // 静态/全局变量的 数组会自动初始化为 0
  int front;       //  起始下标
  int rear;       // 下一个存储元素的下标
  int size;       // 元素个数
  pthread_mutex_t mutex;
  pthread_cond_t not_empty;
  pthread_cond_t not_full;
} BlockQ;

BlockQ *blockq_create(void);
void blockq_destroy(BlockQ *q);

bool blockq_empty(BlockQ *q);
bool blockq_full(BlockQ *q);
void blockq_push(BlockQ *q, E val);
E blockq_pop(BlockQ *q);
E blockq_peek(BlockQ *q);

#endif //CWANG__BLOCKQUEUE_H_
