/**
  ******************************************************************************
  * @file           : SafeQueue.h
  * @author         : xy
  * @brief          : 线程安全的队列（加锁实现）
  * @attention      : None
  * @date           : 2025/3/18
  ******************************************************************************
  */

#ifndef TINYRPC_SRC_UTILS_SAFEQUEUE_H_
#define TINYRPC_SRC_UTILS_SAFEQUEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>

#include <gtest/gtest.h>

template<typename T>
class SafeQueue {
 public:
  void push(const T &value) {
	  std::unique_lock<std::mutex> lock(mtx_);
	  queue_.push(value);
	  cond_.notify_one();
  }

  bool pop(T &value) {
	  std::unique_lock<std::mutex> lock(mtx_);
	  cond_.wait(lock, [this]() {
		return !queue_.empty() || !isRun_;    // true 退出阻塞
	  });
	  if (!isRun_ && queue_.empty()) {
		  return false;
	  }
	  value = queue_.front();
	  queue_.pop();
	  return true;
  }

  void stop() {
	  std::unique_lock<std::mutex> lock(mtx_);
	  isRun_ = false;
	  cond_.notify_all();
  }

 private:
  std::queue<T> queue_;
  std::mutex mtx_;
  std::condition_variable cond_;
  bool isRun_ = true;
 public:
  FRIEND_TEST(SafeQueueTest, PushAndPopSingleThread);
  FRIEND_TEST(SafeQueueTest, PopBlocksUntilPush);
  FRIEND_TEST(SafeQueueTest, MultipleProducersAndConsumers);
  FRIEND_TEST(SafeQueueTest, StopFunctionality);
};

#endif //TINYRPC_SRC_UTILS_SAFEQUEUE_H_
