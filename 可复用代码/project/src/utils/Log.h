/**
  ******************************************************************************
  * @file           : Log.h
  * @author         : xy
  * @brief          : 外部使用日志库，包含该头文件即可
  * @attention      : None
  * @date           : 2025/3/18
  ******************************************************************************
  */

#ifndef TINYRPC_SRC_UTILS_LOG_H_
#define TINYRPC_SRC_UTILS_LOG_H_

#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <thread>
#include "Logger.h"


template<typename T>
std::string to_string_helper(T &&arg) {
	std::ostringstream oss;
	oss << std::forward<T>(arg);
	return oss.str();
}

template<typename... Args>
std::string formatMessage(const std::string &format, Args &&... args) {
	std::vector<std::string> arg_strings = {to_string_helper(std::forward<Args>(args))...};
	std::ostringstream oss;
	size_t arg_index = 0;
	size_t pos = 0;
	size_t placeholder = format.find("{}");

	while (placeholder != std::string::npos) {
		oss << format.substr(pos, placeholder - pos);
		if (arg_index < arg_strings.size()) {
			oss << arg_strings[arg_index++];
		} else {
			oss << "{}"; // 参数不足时保留 "{}"
		}
		pos = placeholder + 2;
		placeholder = format.find("{}", pos);
	}

	oss << format.substr(pos);

	return oss.str();
}


template<typename... Args>
void log(LOGLEVEL level, const char* func, const std::string &format, Args &&... args) {
	auto cur_level = Logger::getInstance()->level();
	if (level < cur_level) {
		return;
	}

	// 获取当前时间
	auto now = std::chrono::system_clock::now();
	auto now_time_t = std::chrono::system_clock::to_time_t(now);
	auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

	std::tm tm_now{};
	localtime_r(&now_time_t, &tm_now);  // 线程安全的时间转换

	// 格式化时间
	std::ostringstream time_stream;
	time_stream << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << now_ms.count();

	// 获取线程 ID
	std::ostringstream thread_stream;
	thread_stream << std::this_thread::get_id();
	std::string thread_id = thread_stream.str();

	std::string message = formatMessage(format, std::forward<Args>(args)...);

	auto level_str = [level](){
	  switch (level) {
		  case LOGLEVEL::INFO: return "INFO";
		  case LOGLEVEL::DEBUG: return "DEBUG";
		  case LOGLEVEL::ERROR: return "ERROR";
		  case LOGLEVEL::FATAL: return "FATAL";
		  default: return "UNKNOWN";
	  }
	}();

	std::ostringstream log_stream;
	log_stream << "[" << time_stream.str() << "] [" << thread_id << "] [" << level_str << "] "
			   << "[" << func << "] " << message;

	Logger::getInstance()->Log(log_stream.str());
}


#define LOG_INFO(format, ...) log(LOGLEVEL::INFO, __PRETTY_FUNCTION__, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) log(LOGLEVEL::DEBUG, __PRETTY_FUNCTION__, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) log(LOGLEVEL::ERROR, __PRETTY_FUNCTION__, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) log(LOGLEVEL::FATAL, __PRETTY_FUNCTION__, format, ##__VA_ARGS__)



#endif //TINYRPC_SRC_UTILS_LOG_H_
