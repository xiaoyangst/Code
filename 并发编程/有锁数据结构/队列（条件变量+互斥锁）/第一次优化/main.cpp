
/******************* SafeQueue Test *********************/
#include "SafeQueue.h"
#include <vector>
#include <iostream>
#include <thread>
#define THREAD_NUM 5
void addNum(SafeQueue<int>& safe_queue);
void delNum(SafeQueue<int>& safe_queue);

int main() {

  SafeQueue<int> safe_queue;

  // 创建 5个 线程  同时添加元素
  std::vector<std::thread> addThread;
  for (int i = 0; i < THREAD_NUM; ++i) {
    addThread.emplace_back(&addNum,std::ref(safe_queue));
  }
  for (int i = 0; i < THREAD_NUM; ++i) {
    addThread[i].join();
  }

  // 创建 5个 线程 同时消费元素
  std::vector<std::thread> delThread;
  for (int i = 0; i < THREAD_NUM; ++i) {
    delThread.emplace_back(&delNum,std::ref(safe_queue));
  }
  for (int i = 0; i < THREAD_NUM; ++i) {
    delThread[i].join();
  }

  return 0;
}


void addNum(SafeQueue<int>& safe_queue){
  for (int i = 0; i < 10; i++) {
    safe_queue.push(i);
  }
}

/*
void delNum(SafeQueue<int>& safe_queue) {
  for (int i = 0; i < 10; i++) {
    try {
      std::cout << *safe_queue.wait_pop() << " ";
    } catch (std::exception &exception) {
      std::cout << exception.what() << std::endl;
    }
  }
  std::cout << std::endl;
}
*/
/*
void delNum(SafeQueue<int>& safe_queue){
  for (int i = 0; i < 10; i++) {
    try {
      std::cout << *safe_queue.try_pop() << " ";
    } catch (std::exception &exception) {
      std::cout<<exception.what()<<std::endl;
    }
  }
  std::cout<<std::endl;
}
*/


void delNum(SafeQueue<int>& safe_queue){
  int num = 0;
  for (int i = 0; i < 10; i++) {
    safe_queue.try_pop(num);
    std::cout <<num<< " ";
  }
  std::cout<<std::endl;
}


/*
void delNum(SafeQueue<int>& safe_queue){
  int num = 0;
  for (int i = 0; i < 10; i++) {
    try {
      safe_queue.wait_pop(num);
      std::cout <<num<< " ";
    } catch (std::exception &exception) {
      std::cout<<exception.what()<<std::endl;
    }
  }
  std::cout<<std::endl;
}
*/


/*
 运行结果：纯属巧合，有些情况也会打乱，但是总量不变，这才是我们应该关心的

  0 1 2 3 4 5 6 7 8 9
  0 1 2 3 4 5 6 7 8 9
  0 1 2 3 4 5 6 7 8 9
  0 1 2 3 4 5 6 7 8 9
  0 1 2 3 4 5 6 7 8 9

  统计结果：各 5 个元素
*/

