#include <iostream>



/******************* SafeStack Test *********************/
#include "SafeStack.h"
#include <vector>
#include <thread>
#define THREAD_NUM 5
void addNum(SafeStack<int>& safe_stack);
void delNum(SafeStack<int>& safe_stack);

int main() {

  SafeStack<int> safe_stack;

  // 创建 5个 线程  同时添加元素
  std::vector<std::thread> addThread;
  for (int i = 0; i < THREAD_NUM; ++i) {
    addThread.emplace_back(&addNum,std::ref(safe_stack));
  }
  for (int i = 0; i < THREAD_NUM; ++i) {
    addThread[i].join();
  }

  // 创建 5个 线程 同时消费元素
  std::vector<std::thread> delThread;
  for (int i = 0; i < THREAD_NUM; ++i) {
    delThread.emplace_back(&delNum,std::ref(safe_stack));
  }
  for (int i = 0; i < THREAD_NUM; ++i) {
    delThread[i].join();
  }

  return 0;
}


void addNum(SafeStack<int>& safe_stack){
  for (int i = 0; i < 10; i++) {
    safe_stack.push(i);
  }
}
void delNum(SafeStack<int>& safe_stack){
  for (int i = 0; i < 10; i++) {
    std::cout<<safe_stack.pop()<<" ";
  }
  std::cout<<std::endl;
}

/*
 运行结果：

  9 8 7 6 5 4 3 2 1 0
  9 5 4 3 2 1 0 9 8 7
  8 6 5 4 3 2 1 0 9 8
  7 7 6 5 4 3 2 1 0 9
  6 8 7 6 5 4 3 2 1 0

  统计结果：各 5 个元素
*/

