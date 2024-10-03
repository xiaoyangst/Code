#include <errno.h>
#include "public.h"
#include "BlockQueue.h"

#define THREAD_NUM 10

BlockQ* data;
pthread_t threadArray[THREAD_NUM];
bool is_open = true;
void* consume(void* num) {
  while(is_open) {
    E num = blockq_pop(data);
    if (num == -1) continue;
    printf("thread %ld consume = %d\n",pthread_self(),num);
    sleep(1);
  }
}

void signal_handler(int signal) {
  blockq_shutdown(data);
  is_open = false;
  blockq_destroy(data);
  for (int i = 0; i < THREAD_NUM; ++i) {
    pthread_cancel(threadArray[i]);
  }
}

int main(int argc, char* argv[]) {
  // 注册信号处理函数
  signal(SIGINT, signal_handler);

  data = blockq_create();
  if (data == NULL) {
    perror("Failed to create BlockQ");
    return 1;
  }

  printf("线程池\n");
  for (int i = 0; i < THREAD_NUM; ++i) { // 线程池 pop
    int err = pthread_create(&threadArray[i], NULL, consume, NULL);
    if (err != 0) {
      error(1, errno, "pthread_create error");
    }
  }

  printf("主线程\n");
  for (int i = 0; i < 50; ++i) { // 主线程 push
    blockq_push(data, i);
  }

  // 主线程等待各个子线程结束。
  for (int i = 0; i < THREAD_NUM; ++i) {
    pthread_join(threadArray[i], NULL);
  }

  // 清理资源
  blockq_destroy(data);
  return 0;
}
