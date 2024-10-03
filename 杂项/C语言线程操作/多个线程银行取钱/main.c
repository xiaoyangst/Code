#include <stdio.h>
#include "public.h"
#define THREAD_NUM 10
typedef struct {
  int id;
  int money;
} Account;

Account acct1 = {1, 100};

// 消费 100 的线程函数
void* consume(void* data){
  printf("Thread %ld run\n",pthread_self());
  pthread_mutex_t* mtx = (pthread_mutex_t*)data;
  pthread_mutex_lock(mtx);
  if (acct1.money >= 100){
    printf("取钱成功，取钱之前卡中金额 %d\n",acct1.money);
    acct1.money = acct1.money - 100;
    printf("取钱成功，取钱之后卡中金额 %d\n",acct1.money);
  }
  pthread_mutex_unlock(mtx);
  pthread_exit(NULL);
}

int main(void) {
  pthread_mutex_t mtx;
  pthread_mutex_init(&mtx,NULL);  // 初始化锁

  // 创建10个线程，同时去acct账号上取100。
  // 要求：任何情况下，都只能有一个线程能取成功。
  pthread_t threadArray[THREAD_NUM];
  for (int i = 0; i < THREAD_NUM; ++i) {
    int err = pthread_create(&threadArray[i],NULL,&consume,&mtx);
    if (err != 0){
      error(1,errno,"pthread_create error");
    }
  }

  // 主线程等待各个子线程结束。
  for (int i = 0; i < THREAD_NUM; ++i) {
    pthread_join(threadArray[i],NULL);
  }

  pthread_mutex_destroy(&mtx);    // 销毁锁
}