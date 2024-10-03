#include <stdio.h>
#include "public.h"

// 线程函数
void* printf_self(){
  printf("thread ID = %ld\n",pthread_self());
}

int main(int argc,char* argv[]) {

  pthread_t thread;
  pthread_create(&thread,NULL,&printf_self,NULL); // 创建一个线程

  // 避免主线程提前 结束
  pthread_join(thread,NULL);

  return 0;
}