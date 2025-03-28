/**
  ******************************************************************************
  * @file           : Zookeeper.h
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2025/3/23
  ******************************************************************************
  */

#ifndef TINYRPC_SRC_UTILS_ZOOKEEPER_H_
#define TINYRPC_SRC_UTILS_ZOOKEEPER_H_

#include <string>
#include <zookeeper/zookeeper.h>

class Zookeeper {
 public:
  Zookeeper() = default;
  ~Zookeeper();
  void start();
  void create(const std::string& path, const std::string& data, int state);
  std::string getData(const std::string& path);
  bool exists(const std::string& path);
 private:
  zhandle_t *m_handle;
};

#endif //TINYRPC_SRC_UTILS_ZOOKEEPER_H_
