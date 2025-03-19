#include "utils/Log.h"
#include <gtest/gtest.h>

TEST(LoggerTest, LoggerBase) {
	std::string name = "xy";
	LOG_INFO("name {} age {}", name, 18);
}

