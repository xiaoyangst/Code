#include <stdio.h>
#include "public.h"

typedef struct {
  int id;
  int money;
  pthread_mutex_t mutex;
} Account;

Account acct1 = {1, 100, PTHREAD_MUTEX_INITIALIZER};
Account acct2 = {2, 900, PTHREAD_MUTEX_INITIALIZER};

void* Transfer100(void* data){
  pthread_mutex_lock(&acct1.mutex);
  pthread_mutex_lock(&acct2.mutex);
  if (acct1.money >= 100){
    acct1.money -= 100;
    acct2.money += 100;
  }
  pthread_mutex_unlock(&acct1.mutex);
  pthread_mutex_unlock(&acct2.mutex);
  pthread_exit(NULL);
}

void* Transfer900(void* data){
  pthread_mutex_lock(&acct1.mutex);
  pthread_mutex_lock(&acct2.mutex);
  if (acct2.money >= 900){
    acct2.money -= 900;
    acct1.money += 900;
  }
  pthread_mutex_unlock(&acct1.mutex);
  pthread_mutex_unlock(&acct2.mutex);
  pthread_exit(NULL);
}

int main(void) {
  printf("acct1 money = %d\n",acct1.money);
  printf("acct2 money = %d\n",acct2.money);
  // 创建两个线程：
  pthread_t t1;
  pthread_t t2;
  // 其中一个线程：从acct1转账100给acct2
  pthread_create(&t1,NULL,Transfer100,NULL);
  // 同时另一个线程：从acct2转账900给acct1
  pthread_create(&t2,NULL,Transfer900,NULL);
  // 要求：任何情况下，都避免死锁现象！ 按顺序取锁即可
  // 主线程等待各个子线程结束。
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);

  printf("acct1 money = %d\n",acct1.money);
  printf("acct2 money = %d\n",acct2.money);

  pthread_mutex_destroy(&acct1.mutex);
  pthread_mutex_destroy(&acct2.mutex);
}