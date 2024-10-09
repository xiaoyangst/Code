/******************* SafeLinkList Test *********************/

#include <iostream>
#include "SafeLinkList.h"
#include <string>
#include <vector>
#include <thread>

void testSafeLinkList()  {  // 功能测试
  SafeLinkList<std::string> list;

  // 向链表中插入数据
  list.push_front("Hello");
  list.push_front("World");
  list.push_front("Safe");
  list.push_front("LinkList");

  std::cout << "Elements in the list:" << std::endl;

  // 遍历并打印所有数据
  list.for_each([](const std::string& data) {
    std::cout << data << std::endl;
  });

  // 查找第一个满足条件的元素
  auto found = list.find_first_if([](const std::string& data) {
    return data == "World";
  });

  if (found) {
    std::cout << "Found: " << *found << std::endl;
  } else {
    std::cout << "Element not found!" << std::endl;
  }

  // 删除指定元素
  list.remove_if([](const std::string& data) {
    return data == "Safe" || data == "World";
  });

  std::cout << "Elements after removal:" << std::endl;

  // 再次遍历链表，确保数据已经被正确删除
  list.for_each([](const std::string& data) {
    std::cout << data << std::endl;
  });
}

SafeLinkList<int> safe_list;

void insert_elements(int start, int end) {
  for (int i = start; i <= end; ++i) {
    safe_list.push_front(i);
  }
}

void find_elements(int max_val) {
  for (int i = 0; i < max_val; ++i) {
    auto found = safe_list.find_first_if([i](int data) {
      return data == i;
    });
    if (found) {
      std::cout << "Found: " << *found << "\n";
    } else {
      std::cout << "Not found: " << i << "\n";
    }
  }
}

void remove_elements(int max_val) {
  for (int i = 0; i < max_val; ++i) {
    safe_list.remove_if([i](int data) {
      if (data == i) {
        std::cout << "Removing: " << data << "\n";
        return true;
      }
      return false;
    });
  }
}

int main() {
  const int num_threads = 5;
  const int num_elements = 5;

  // 创建线程来插入数据
  std::vector<std::thread> threads;
  printf("插入元素：\n");
  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back(insert_elements, i * num_elements, (i + 1) * num_elements - 1);
  }
  printf("\n");
  // 等待所有插入线程结束
  for (auto& th : threads) {
    th.join();
  }

  // 打印所有元素
  safe_list.for_each([](int data) {
    std::cout << data << " ";
  });
  std::cout << "\n";

  // 创建查找线程
  threads.clear();
  printf("查找元素：\n");
  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back(find_elements, num_elements * num_threads);
  }
  printf("\n");
  // 等待所有查找线程结束
  for (auto& th : threads) {
    th.join();
  }

  // 创建删除线程
  threads.clear();
  printf("删除元素：\n");
  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back(remove_elements, num_elements * num_threads);
  }
  printf("\n");
  // 等待所有删除线程结束
  for (auto& th : threads) {
    th.join();
  }

  std::cout << "List after removal:\n";
  safe_list.for_each([](int data) {
    std::cout << data << " ";
  });
  std::cout << "\n";

  return 0;
}
