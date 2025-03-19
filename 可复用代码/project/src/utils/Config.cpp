/**
  ******************************************************************************
  * @file           : Config.cpp
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2025/3/18
  ******************************************************************************
  */

#include <mutex>
#include <cstdlib>
#include <fstream>
#include <cassert>
#include "Config.h"

Config *Config::instance_ = nullptr;

/**
 * @brief 获取单例对象
 * @return Config实例化对象
 */
Config *Config::getInstance() {
	static std::once_flag flag;
	std::call_once(flag, [&] {
	  instance_ = new Config(kConfigPath);
	  atexit(destroy);
	});
	return instance_;
}

Config::Config(const std::string &config_path) {
	parse(config_path);
}

/**
 * @brief 解析配置文件并加载到内存中
 * @param path
 */
void Config::parse(const std::string &path) {
	std::ifstream ifs(path);
	assert(ifs.is_open() == true);

	std::string line;
	while (std::getline(ifs, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}
		auto pos = line.find('=');
		if (pos == std::string::npos) {
			continue;
		}
		std::string key = line.substr(0, pos);
		trim(key);
		std::string value = line.substr(pos + 1);
		trim(value);
		config_map_[key] = value;
	}
}

/**
 * @brief 获取配置文件信息（value）
 * @param key
 * @return std::optional<std::string>
 */
std::optional<std::string> Config::get(const std::string &key) const {
	auto it = config_map_.find(key);
	if (it == config_map_.end()) {
		return std::nullopt;
	}
	return it->second;
}

/**
 * @brief 回收单例资源
 */
void Config::destroy() {
	if (instance_) {
		delete instance_;
		instance_ = nullptr;
	}
}

/**
 * @brief 移除左右两边多余的空格和换行符
 * @attention 考虑到 Windows 和 Linux 的换行符不同，所以这里要一起考虑
 * @param content
 */
void Config::trim(std::string &content) {
	if (content.empty()) {
		return;
	}
	// 找到 第一个 不是空格、制表符、回车或换行符的字符位置
	content.erase(0, content.find_first_not_of(" \t\r\n"));
	// 找到 最后一个 不是空格、制表符、回车或换行符的字符位置
	content.erase(content.find_last_not_of(" \t\r\n") + 1);
}



