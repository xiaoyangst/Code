#include "utils/Config.h"
#include <gtest/gtest.h>

TEST(ConfigTest,ConfigBase){
	auto ip = Config::getInstance()->get("ip");
	EXPECT_EQ(ip, "127.0.0.1");
	auto port = Config::getInstance()->get("port");
	EXPECT_EQ(port, "3306");
	auto username = Config::getInstance()->get("username");
	EXPECT_EQ(username, "root");
	auto password = Config::getInstance()->get("password");
	EXPECT_EQ(password, "123456");
}
