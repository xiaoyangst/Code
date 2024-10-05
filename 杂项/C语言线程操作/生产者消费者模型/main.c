#include "public.h"
#include "BlockQueue.h"

typedef struct threadpool{
  pthread_t* threads;
  int num;
  BlockQ* safeQueue;
}ThreadPool;

// 线程函数
void* start_routine(void* arg){ // 循环消费，没有可消费对象就阻塞
  ThreadPool* pool = (ThreadPool*)arg;

  pthread_t tid = pthread_self();
  for(;;) {
    int task_id = blockq_pop(pool->safeQueue);
    if (task_id == -1) {// 退出点
      printf("0x%lx: exit\n", tid);
      pthread_exit(NULL);
    }
    printf("0x%lx: execute task %d\n", tid, task_id);
    sleep(3);	// 模拟执行任务
    printf("0x%lx: task %d done\n", tid, task_id);
  }

  return NULL;
}

// 创建线程池

ThreadPool* threads_create(int num){
  ThreadPool* pool = (ThreadPool*) malloc(sizeof(ThreadPool) * num);

  pool->num = num;
  pool->safeQueue = blockq_create();
  pool->threads = (pthread_t*) malloc(sizeof(pthread_t) * num);

  for (int i = 0; i < num; ++i) {
    pthread_create(&pool->threads[i],NULL,start_routine,pool);
  }

  return pool;
}

void threads_destroy(ThreadPool* pool){
  blockq_destroy(pool->safeQueue);
  free(pool->threads);
  free(pool);
}

void close_pool(BlockQ* queue){
  blockq_push(queue,-1);
}

int main(void){

  ThreadPool* pool = threads_create(10);

  for (int i = 1; i <= 100; i++) {  // 生产
    blockq_push(pool->safeQueue, i);
  }

  // 优雅退出
  for (int i = 0; i < pool->num; ++i) {
    close_pool(pool->safeQueue);
  }

  for (int i = 0; i < pool->num; ++i) {
    pthread_join(pool->threads[i],NULL);
  }

  threads_destroy(pool);
  return 0;
}