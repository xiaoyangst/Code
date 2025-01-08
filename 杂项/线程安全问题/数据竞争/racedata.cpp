#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

// 全局变量
int global_variable = 0;

void *thread_function(void *arg) {
    // 每个线程都会对全局变量进行自增操作
    for (int i = 0; i < 1000000; i++) {
        global_variable++;
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // 创建两个线程
    if (pthread_create(&thread1, NULL, thread_function, NULL)!= 0) {
        perror("Thread 1 creation failed");
        return 1;
    }
    if (pthread_create(&thread2, NULL, thread_function, NULL)!= 0) {
        perror("Thread 2 creation failed");
        return 1;
    }

    // 等待线程结束
    if (pthread_join(thread1, NULL)!= 0) {
        perror("Thread 1 join failed");
        return 1;
    }
    if (pthread_join(thread2, NULL)!= 0) {
        perror("Thread 2 join failed");
        return 1;
    }

    // 打印全局变量的值
    printf("Global variable value: %d\n", global_variable);
    return 0;
}
