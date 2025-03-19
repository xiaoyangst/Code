cmake_minimum_required(VERSION 3.16)
project(TinyRpc)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(test)
add_executable(TinyRpc main.cpp)
