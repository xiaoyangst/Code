//
// Created by xy on 2024/10/21.
//

#ifndef CONCURRENCESAFE_CONCURRENCE_SAFESEARCHTABLE_H_
#define CONCURRENCESAFE_CONCURRENCE_SAFESEARCHTABLE_H_

#include <list>
#include <shared_mutex>
#include <algorithm>
#include <map>
#include <list>
#include <shared_mutex>
#include <algorithm>
#include <map>
#include <vector>
#include <memory>

#include <list>
#include <shared_mutex>
#include <algorithm>
#include <map>
#include <vector>
#include <memory>

template<typename Key, typename Value, typename Hash = std::hash<Key>>
class SafeSearchTable {
 private:
  class bucket_type {
    friend class SafeSearchTable;

   private:
    typedef std::pair<Key, Value> bucket_value;
    typedef std::list<bucket_value> bucket_data;
    typedef typename bucket_data::iterator bucket_iterator;
    typedef typename bucket_data::const_iterator const_bucket_iterator;

    bucket_data data_;
    mutable std::shared_mutex mutex_;  // 共享互斥量，可以实现读写锁，保证读的时候可以并发读

    // 非 const 情况下，返回 iterator
    bucket_iterator find_entry_for(Key const& key) {
      return std::find_if(data_.begin(), data_.end(), [&](bucket_value const& item) {
        return item.first == key;
      });
    }

    // const 情况下，返回 const_iterator
    const_bucket_iterator find_entry_for(Key const& key) const {
      return std::find_if(data_.begin(), data_.end(), [&](bucket_value const& item) {
        return item.first == key;
      });
    }

   public:
    Value value_for(Key const& key, Value const& default_value) const {  // 通过 key 获取 value
      std::shared_lock<std::shared_mutex> lock(mutex_);
      const_bucket_iterator const found_entry = find_entry_for(key);
      return (found_entry == data_.end()) ? default_value : found_entry->second;
    }

    void add_or_update_mapping(Key const& key, Value const& value) {  // 修改函数名为 add_or_update_mapping
      std::unique_lock<std::shared_mutex> lock(mutex_);
      bucket_iterator const found_entry = find_entry_for(key);
      if (found_entry == data_.end()) {
        data_.push_back(bucket_value(key, value));
      } else {
        found_entry->second = value;
      }
    }

    void remove_mapping(Key const& key) {
      std::unique_lock<std::shared_mutex> lock(mutex_);
      bucket_iterator const found_entry = find_entry_for(key);
      if (found_entry != data_.end()) {
        data_.erase(found_entry);
      }
    }

  };

 private:
  // 用 vector 存储桶类型
  std::vector<std::unique_ptr<bucket_type>> buckets;
  // hash<Key> 哈希表 用来根据 key 生成哈希值
  Hash hasher;

  // 根据 key 生成数字，并对桶的大小取余得到下标，根据下标返回对应的桶智能指针
  bucket_type& get_bucket(Key const& key) const {
    std::size_t const bucket_index = hasher(key) % buckets.size();
    return *buckets[bucket_index];
  }

 public:
  SafeSearchTable(SafeSearchTable const&) = delete;
  SafeSearchTable& operator=(SafeSearchTable const&) = delete;

  SafeSearchTable(unsigned num_buckets = 19, Hash const& hasher_ = Hash())
      : buckets(num_buckets), hasher(hasher_) {
    for (unsigned i = 0; i < num_buckets; ++i) {
      buckets[i].reset(new bucket_type);
    }
  }

  Value value_for(Key const& key, Value const& default_value = Value()) const {
    return get_bucket(key).value_for(key, default_value);
  }

  void add_or_update_mapping(Key const& key, Value const& value) {
    get_bucket(key).add_or_update_mapping(key, value);
  }

  void remove_mapping(Key const& key) {
    get_bucket(key).remove_mapping(key);
  }

  std::map<Key, Value> get_map() const {
    std::vector<std::unique_lock<std::shared_mutex>> locks;
    for (unsigned i = 0; i < buckets.size(); ++i) {
      locks.push_back(std::unique_lock<std::shared_mutex>(buckets[i]->mutex_));
    }
    std::map<Key, Value> res;
    for (unsigned i = 0; i < buckets.size(); ++i) {
      typename bucket_type::bucket_iterator it = buckets[i]->data_.begin();
      for (; it != buckets[i]->data_.end(); ++it) {
        res.insert(*it);
      }
    }
    return res;
  }
};





#endif //CONCURRENCESAFE_CONCURRENCE_SAFESEARCHTABLE_H_
