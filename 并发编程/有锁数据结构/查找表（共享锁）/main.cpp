/******************* SafeSearchTable Test *********************/

#include <iostream>
#include <thread>
#include <vector>
#include "SafeSearchTable.h"  // 假设 SafeSearchTable 的代码已经放在 SafeSearchTable.h 中

void test_single_thread() {
  SafeSearchTable<int, std::string> table(10);  // 创建有10个桶的哈希表

  // 添加或更新键值对
  table.add_or_update_mapping(1, "Alice");
  table.add_or_update_mapping(2, "Bob");
  table.add_or_update_mapping(3, "Charlie");

  // 通过键获取值
  std::cout << "Key 1: " << table.value_for(1, "Not Found") << std::endl;
  std::cout << "Key 2: " << table.value_for(2, "Not Found") << std::endl;
  std::cout << "Key 3: " << table.value_for(3, "Not Found") << std::endl;
  std::cout << "Key 4: " << table.value_for(4, "Not Found") << std::endl;  // 键不存在

  // 删除键值对
  table.remove_mapping(2);

  // 再次检查键 2 是否存在
  std::cout << "Key 2 after removal: " << table.value_for(2, "Not Found") << std::endl;

  // 获取所有键值对
  std::map<int, std::string> all_data = table.get_map();
  std::cout << "All data in the table: " << std::endl;
  for (const auto& entry : all_data) {
    std::cout << "Key: " << entry.first << ", Value: " << entry.second << std::endl;
  }
}

void test_multi_thread() {
  SafeSearchTable<int, std::string> table(10);  // 创建有10个桶的哈希表

  // 在线程中执行写操作
  auto writer = [&table](int id, const std::string& value) {
    for (int i = 0; i < 5; ++i) {
      table.add_or_update_mapping(id * 5 + i, value + std::to_string(i));
    }
  };

  // 在线程中执行读操作
  auto reader = [&table](int id) {
    for (int i = 0; i < 5; ++i) {
      std::cout << "Reader " << id << " reading key " << id * 5 + i
                << ": " << table.value_for(id * 5 + i, "Not Found") << std::endl;
    }
  };

  // 创建多个线程
  std::vector<std::thread> threads;
  for (int i = 0; i < 4; ++i) {
    threads.emplace_back(writer, i, "Thread" + std::to_string(i));
    threads.emplace_back(reader, i);
  }

  // 等待所有线程完成
  for (auto& th : threads) {
    th.join();
  }

  // 获取所有键值对
  std::map<int, std::string> all_data = table.get_map();
  std::cout << "All data in the table after multi-thread operations: " << std::endl;
  for (const auto& entry : all_data) {
    std::cout << "Key: " << entry.first << ", Value: " << entry.second << std::endl;
  }
}

int main() {
  std::cout << "Single-threaded test:" << std::endl;
  test_single_thread();

  std::cout << "\nMulti-threaded test:" << std::endl;
  test_multi_thread();

  return 0;
}
