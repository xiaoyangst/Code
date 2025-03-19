/**
  ******************************************************************************
  * @file           : Logger.h
  * @author         : xy
  * @brief          : 异步日志
  * @attention      : None
  * @date           : 2025/3/18
  ******************************************************************************
  */

#ifndef TINYRPC_SRC_UTILS_LOGGER_H_
#define TINYRPC_SRC_UTILS_LOGGER_H_

#include <atomic>
#include <thread>
#include <fstream>
#include <gtest/gtest.h>
#include "SafeQueue.h"

enum class LOGLEVEL {
  INFO,
  DEBUG,
  WARN,
  FATAL
};

class Logger {
 public:
  static Logger *getInstance();
  Logger();
  ~Logger();
  void Log(const std::string &log);
 public:
  void setLevel(LOGLEVEL level) { log_level_ = level; };
  LOGLEVEL level() { return log_level_; };
 private:
  static std::string getCurTime();
  void writeLog();
  static void destroy();
 private:
  static Logger *instance_;
  SafeQueue<std::string> queue_;
  std::thread work_thread_;
  std::ofstream log_file_;
  std::atomic<bool> is_exit_ = false;
  LOGLEVEL log_level_ = LOGLEVEL::INFO;
 private:

 public:
  FRIEND_TEST(LoggerTest, LoggerBase);
};

#endif //TINYRPC_SRC_UTILS_LOGGER_H_
