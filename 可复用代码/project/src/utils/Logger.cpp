/**
  ******************************************************************************
  * @file           : Logger.cpp
  * @author         : xy
  * @brief          : None
  * @attention      : None
  * @date           : 2025/3/18
  ******************************************************************************
  */

#include <cstdlib>
#include "Logger.h"
#include "Config.h"

Logger *Logger::instance_ = nullptr;

Logger *Logger::getInstance() {
	static std::once_flag flag;
	std::call_once(flag, [&] {
	  instance_ = new Logger();
	  atexit(destroy);
	});
	return instance_;
}

Logger::Logger() {
	auto logPath = Config::getInstance()->get("log_path");
	assert(logPath != std::nullopt);
	auto new_path = logPath.value() + getCurTime() + ".log";
	log_file_.open(new_path, std::ios::app);

	work_thread_ = std::thread(&Logger::writeLog, this);
}

Logger::~Logger() {
	queue_.stop();
	if (work_thread_.joinable()) {
		work_thread_.join();
	}
	if (log_file_.is_open()) {
		log_file_.close();
	}
}

void Logger::Log(const std::string &log) {
	queue_.push(log);
}

void Logger::writeLog() {
	std::string message;
	while (queue_.pop(message)) {
		 std::cout << message << std::endl;
		log_file_ << message << "\n";
		log_file_.flush();
	}
}

std::string Logger::getCurTime() {
	time_t now = time(nullptr);
	struct tm *t = localtime(&now);
	char str[64] = {0};
	strftime(str, sizeof(str), "%Y%m%d%H%M%S", t);
	return {str};
}

void Logger::destroy() {
	if (instance_) {
		delete instance_;
		instance_ = nullptr;
	}
}

