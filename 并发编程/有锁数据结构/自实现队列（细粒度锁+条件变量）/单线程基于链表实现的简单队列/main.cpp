#include "StrongestSafeQueue.h"
/******************* StrongestSafeQueue Test *********************/

#include <iostream>
#include "StrongestSafeQueue.h"

int main() {
  StrongestSafeQueue<int> queue;

  // 测试 push 操作
  std::cout << "Pushing elements into the queue:" << std::endl;
  for (int i = 1; i <= 5; ++i) {
    std::cout << "Pushing: " << i << std::endl;
    queue.push(i);
  }

  // 测试 try_pop 操作
  std::cout << "\nPopping elements from the queue:" << std::endl;
  for (int i = 0; i < 5; ++i) {
    auto item = queue.try_pop();
    if (item) {
      std::cout << "Popped: " << *item << std::endl;
    } else {
      std::cout << "Queue is empty." << std::endl;
    }
  }

  // 尝试在队列为空时进行 pop 操作
  std::cout << "\nAttempting to pop from an empty queue:" << std::endl;
  auto item = queue.try_pop();
  if (!item) {
    std::cout << "Queue is empty." << std::endl;
  }

  return 0;
}


/*

Pushing elements into the queue:
Pushing: 1
Pushing: 2
Pushing: 3
Pushing: 4
Pushing: 5

Popping elements from the queue:
Popped: 1
Popped: 2
Popped: 3
Popped: 4
Popped: 5

Attempting to pop from an empty queue:
Queue is empty.


*/