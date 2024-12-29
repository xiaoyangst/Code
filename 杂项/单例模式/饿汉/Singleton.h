//
// Created by xy on 2024/11/22.
//

#ifndef STUDY__SINGLETON_H_
#define STUDY__SINGLETON_H_
#include <iostream>
#include <memory>
class Singleton{
 public:
  static Singleton* getInstance(){
    return m_instance;
  }

  void printAddr(){
   std::cout << m_instance << std::endl;
  }

  // 删除拷贝构造和拷贝赋值
  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;
  // 删除移动构造和移动赋值构造
  Singleton(Singleton&&) = delete;
  Singleton& operator=(Singleton&&) = delete;

 private:
  Singleton() = default;
  ~Singleton() = default;
 private:
  static Singleton* m_instance;
};
Singleton* Singleton::m_instance = new Singleton(); // 提前创建，调用 getInstance 必然已经初始化成功
#endif //STUDY__SINGLETON_H_
