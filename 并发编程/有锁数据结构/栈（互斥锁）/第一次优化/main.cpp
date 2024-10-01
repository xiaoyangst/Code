


/******************* SafeStack Test *********************/
#include "SafeStack.h"
#include <vector>
#include <iostream>
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

//void delNum(SafeStack<int>& safe_stack){
//  for (int i = 0; i < 10; i++) {
//    try {
//      std::cout << *safe_stack.pop() << " ";
//    } catch (std::exception &exception) {
//      std::cout<<exception.what()<<std::endl;
//    }
//  }
//  std::cout<<std::endl;
//}

void delNum(SafeStack<int>& safe_stack){
  int num = 0;
  for (int i = 0; i < 10; i++) {
    try {
      safe_stack.pop(num);
      std::cout <<num<< " ";
    } catch (std::exception &exception) {
      std::cout<<exception.what()<<std::endl;
    }
  }
  std::cout<<std::endl;
}

/*
 运行结果：

  9 8 7 6 5 4 3 2 1 0
  8 7 6 5 4 3 2 1 0 9
  8 7 6 5 4 3 2 1 0 9
  8 7 6 5 4 3 2 1 0 9
  9 8 7 6 5 4 3 2 1 0

  统计结果：各 5 个元素
*/

