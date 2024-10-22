/**
  ******************************************************************************
  * @file           : Singleton.h
  * @author         : xy
  * @brief          : 线程安全的单例
  * @attention      : None
  * @date           : 2024/10/15
  ******************************************************************************
  */

#ifndef SHEEPCHATSERVER_INCLUDE_SINGLETON_H_
#define SHEEPCHATSERVER_INCLUDE_SINGLETON_H_

#include "header.h"

template<typename T>
class Singleton{
 public:
  static std::shared_ptr<T> getInstance(){
    static std::once_flag flag_;
    std::call_once(flag_,[&](){
      instance_ = std::shared_ptr<T>(new T);
    });
    return instance_;
  }

  Singleton(const Singleton<T>&) = delete;
  Singleton& operator=(const Singleton<T>&) = delete;

  ~Singleton(){
    std::cout<<"~Singleton"<<std::endl;
  }
 protected:
  Singleton() = default;
  static std::shared_ptr<T> instance_;
};

template<typename T>
std::shared_ptr<T> Singleton<T>::instance_ = nullptr;

#endif //SHEEPCHATSERVER_INCLUDE_SINGLETON_H_

/*
 * 为什么 std::shared_ptr<T>(new T) 创建当你，而不是 std::make_shared 来创建 ？
 *
 */