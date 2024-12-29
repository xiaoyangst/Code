/**
  ******************************************************************************
  * @file           : LRU.h
  * @author         : xy
  * @brief          : LRU 缓存结构
  * @attention      : 非线程安全
  * @date           : 2024/11/22
  ******************************************************************************
  */

#ifndef STUDY__LRU_H_
#define STUDY__LRU_H_

#include <unordered_map>
#include <list>
#include <iostream>
template <typename Key,typename Value>
class LRU {
 public:
  explicit LRU(int capacity):m_capacity(capacity){}
  std::optional<Value> get(Key key){
    // 查询用 m_map
    auto item = m_map.find(key);
    if (item != m_map.end()){
      // 查询之后记得更新 m_cache  --> splice 函数 --> 移动到头部
      m_cache.splice(m_cache.begin(),m_cache,item->second);
      return item->second->second;
    }
    return std::nullopt;  // 不存在的返回标志
  }
  void put(Key key,Value value){
    // 检查 是否已经存在
    auto item = m_map.find(key);
    if (item != m_map.end()){
      // 更新 m_cache
      m_cache.splice(m_cache.begin(),m_cache,item->second);
      // 更新 value
      item->second->second = value;
      return;
    }
    if (m_cache.size() == m_capacity){ // 触发淘汰机制
      auto delItem = m_cache.back();
      m_cache.pop_back();
      m_map.erase(delItem.first);
    }
    m_cache.emplace_front(key,value);
    m_map[key] = m_cache.begin(); // 因为是在 头部插入，也就可以直接获取
  }

  void show(){
    for (auto item : m_cache) {
      std::cout <<  "("
                << item.first << "," <<item.second
                <<  ")" <<"--->";
    }
    std::cout<<"NULL"<<std::endl;
  }
 private:
  int m_capacity;
  std::list<std::pair<Key,Value>> m_cache;
  std::unordered_map<Key,typename std::list<std::pair<Key,Value>>::iterator> m_map;
};

#endif //STUDY__LRU_H_

