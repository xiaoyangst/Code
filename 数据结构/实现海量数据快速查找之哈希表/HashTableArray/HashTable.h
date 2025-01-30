#ifndef INTERVIEW_LINEARPROBING_HASHTABLE_H_
#define INTERVIEW_LINEARPROBING_HASHTABLE_H_

#include <vector>
#include <iostream>
#include <functional>
#include <optional>

template<typename Key, typename Val>
class HashTableArray {
 private:
  struct Entry {    // 单个元素
    Key key;
    Val value;
    bool isActive = false;  // 标记是否已经被使用

    Entry(const Key &k, const Val &v) : key(k), value(v), isActive(true) {}
  };

  using Table = std::vector<std::optional<Entry>>;

 public:
  HashTableArray() : table_(INIT_TABLE_SIZE), elements_(0) {}

  // 插入
  void insert(const Key &key, const Val &val) {
      if (isRehashNeeded()) {
          rehash();
      }

      size_t index = findSlot(key);

      if (!table_[index].has_value()) {
          table_[index] = Entry(key, val);
          ++elements_;
      } else {
          table_[index]->value = val; // 存在则更新
      }
  }

  // 删除
  bool remove(const Key &key) {
      size_t index = findSlot(key);

      // 有值 且 不是删除状态
      if (table_[index].has_value() && table_[index]->isActive) {
          table_[index]->isActive = false;
          --elements_;
          return true;
      }
      return false;
  }

  // 查找
  bool find(const Key &key, Val &val) const {
      size_t index = findSlot(key);
      if (table_[index].has_value() && table_[index]->isActive) {
          val = table_[index]->value;
          return true;
      }
      return false;
  }

  // 打印哈希表内容
  void print() const {
      for (size_t i = 0; i < table_.size(); ++i) {
          if (table_[i].has_value()) {
              if (table_[i]->isActive) {
                  std::cout << "Index " << i << ": [" << table_[i]->key << ": " << table_[i]->value << "]\n";
              } else {
                  std::cout << "Index " << i << ": [DELETED]\n";
              }
          } else {
              std::cout << "Index " << i << ": [EMPTY]\n";
          }
      }
      std::cout << "===========================================\n";
  }

 private:
  // 哈希函数
  size_t hashFun(const Key &key) const {
      return std::hash<Key>{}(key) % table_.size();
  }

  // 线性探测寻找插入/查找位置
  size_t findSlot(const Key &key) const {
      size_t index = hashFun(key);
      while (table_[index].has_value() && table_[index]->key != key) {
          index = (index + 1) % table_.size();
      }
      return index;
  }

  // 是否需要 rehash
  bool isRehashNeeded() const {
      return static_cast<double>(elements_) / table_.size() >= LOAD_FACTOR_THRESHOLD;
  }

  // rehash
  void rehash() {
      size_t new_size = table_.size() * 2;
      Table new_table(new_size);

      for (auto &entry : table_) {
          if (entry.has_value() && entry->isActive) {
              size_t index = std::hash<Key>{}(entry->key) % new_size;
              while (new_table[index].has_value()) {
                  index = (index + 1) % new_size;
              }
              new_table[index] = std::move(entry);
          }
      }

      table_ = std::move(new_table);
  }

 private:
  const size_t INIT_TABLE_SIZE = 10;
  const double LOAD_FACTOR_THRESHOLD = 0.75;
  Table table_;
  size_t elements_;
};

#endif // INTERVIEW_LINEARPROBING_HASHTABLE_H_
