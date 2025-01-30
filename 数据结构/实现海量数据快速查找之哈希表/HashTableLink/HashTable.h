#ifndef INTERVIEW_HASHTABLELINK_HASHTABLE_H_
#define INTERVIEW_HASHTABLELINK_HASHTABLE_H_

#include <vector>
#include <list>
#include <iostream>
#include <functional> // std::hash

template<typename Key, typename Val>
class HashTableLink {
  using HashTable = std::vector<std::list<std::pair<Key, Val>>>;

 public:
  HashTableLink() : hash_table_(INIT_TABLE_SIZE), elements_(0) {}

  // 插入
  void insert(const Key &key, const Val &val) {
      if (isRehash()) {
          rehash();
      }

      auto index = hashFun(key);
      auto &find_list = hash_table_[index];

      for (auto &item : find_list) {
          if (item.first == key) {
              item.second = val;
              return;
          }
      }

      find_list.emplace_back(key, val);
      ++elements_;
  }

  // 删除
  bool remove(const Key &key) {
      auto index = hashFun(key);
      auto &find_list = hash_table_[index];

      for (auto it = find_list.begin(); it != find_list.end(); ++it) {
          if (it->first == key) {
              find_list.erase(it);
              --elements_;
              return true;
          }
      }

      return false;
  }

  // 查找
  bool find(const Key &key, Val &val) const {
      auto index = hashFun(key);
      const auto &find_list = hash_table_[index];

      for (const auto &item : find_list) {
          if (item.first == key) {
              val = item.second;
              return true;
          }
      }
      return false;
  }

  // 打印哈希表内容
  void print() const {
      for (size_t i = 0; i < hash_table_.size(); ++i) {
          std::cout << "Bucket " << i << ": ";
          for (const auto &pair : hash_table_[i]) {
              std::cout << "[" << pair.first << ": " << pair.second << "] ";
          }
          std::cout << std::endl;
      }
      std::cout << "=======================================================" << std::endl;
  }

 private:
  // 哈希函数
  int hashFun(const Key &key) const {
      return std::hash<Key>{}(key) % hash_table_.size();
  }

  // 是否需要 rehash
  bool isRehash() const {
      return (static_cast<double>(elements_) / hash_table_.size()) >= LOAD_FACTOR_THRESHOLD;
  }

  // rehash
  void rehash() {
      //　两倍扩容
      auto new_size = hash_table_.size() * 2;
      HashTable new_hash_table(new_size);

      // 旧哈希表中元素 通过重新计算 哈希，并加入到 新哈希表中
      for (const auto &bucket : hash_table_) {
          for (const auto &pair : bucket) {
              int newIndex = std::hash<Key>{}(pair.first) % new_size;   // 容易犯错，误认为可以直接调用 hashFun
              new_hash_table[newIndex].emplace_back(pair);
          }
      }

      hash_table_ = std::move(new_hash_table);
  }

 private:
  const int INIT_TABLE_SIZE = 10;               // 初始哈希表大小
  const double LOAD_FACTOR_THRESHOLD = 0.75;    // 负载因子阈值
  HashTable hash_table_;                        // 哈希表
  size_t elements_;                             // 元素数量
};

#endif //INTERVIEW_HASHTABLELINK_HASHTABLE_H_
