/**
  ******************************************************************************
  * @file           : Config.h
  * @author         : xy
  * @brief          : 解析 .ini 配置文件
  * @attention      : 只用于查询，不涉及增加，线程安全
  * @date           : 2025/3/18
  ******************************************************************************
  */

#ifndef TINYRPC_UTILS_CONFIG_H_
#define TINYRPC_UTILS_CONFIG_H_

#include <unordered_map>
#include <gtest/gtest.h>

const std::string kConfigPath = "config/config.ini";

class Config {
 public:
  static Config *getInstance();
  std::optional<std::string> get(const std::string &key) const;
 private:
  explicit Config(const std::string &config_path);
  void parse(const std::string &path);
  static void destroy();
  static void trim(std::string& content);
 private:
  static Config *instance_;
  std::unordered_map<std::string, std::string> config_map_;
 public:
  FRIEND_TEST(ConfigTest, ConfigBase);
};

#endif //TINYRPC_UTILS_CONFIG_H_
