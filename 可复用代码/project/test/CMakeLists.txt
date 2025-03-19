cmake_minimum_required(VERSION 3.16)
project(Test)

# debug 信息
set(CMAKE_BUILD_TYPE Debug)

add_executable(ConfigTest ${CMAKE_SOURCE_DIR}/src/utils/Config.cpp ConfigTest.cpp)
target_link_libraries(ConfigTest PRIVATE GTest::GTest GTest::Main pthread)
target_include_directories(ConfigTest PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(LogTest ${CMAKE_SOURCE_DIR}/src/utils/Logger.cpp
        ${CMAKE_SOURCE_DIR}/src/utils/Config.cpp
        LogTest.cpp)
target_link_libraries(LogTest PRIVATE GTest::GTest GTest::Main pthread)
target_include_directories(LogTest PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_executable(SafeQueueTest SafeQueueTest.cpp)
target_link_libraries(SafeQueueTest PRIVATE GTest::GTest GTest::Main pthread)
target_include_directories(SafeQueueTest PRIVATE ${CMAKE_SOURCE_DIR}/src)


# 注册测试
include(GoogleTest)
gtest_discover_tests(ConfigTest)
gtest_discover_tests(LogTest)
gtest_discover_tests(SafeQueueTest)