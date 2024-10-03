#include <stdio.h>
#include "public.h"

#define LEN 128

// 线程函数
void* printf_self(void * data){
  int* range = (int*) data;
  int* re = (int*)malloc(sizeof(int));  // 因为要传递出去，返回值必须得是堆空间（重点！！！）
  for (int i = 0; i <= *range; ++i) {
    *re += i;
  }
  pthread_exit(re);  // 传递返回值
}

int main(int argc,char* argv[]) {

  pthread_t thread;
  int range = 100;
  pthread_create(&thread,NULL,&printf_self,&range); // 创建一个线程

  // 避免主线程提前 结束
  int* retval = 0;  // pthread_join 的 retval 参数类型是 void**， void* 代表参数类型得是一个指针（即堆上的一块内存）；同时还是传出参数，需要传递地址，在加上 *
  pthread_join(thread, (void **)&retval);

  printf("retval = %d\n",*retval);

  return 0;
}