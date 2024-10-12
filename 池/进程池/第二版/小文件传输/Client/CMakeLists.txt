cmake_minimum_required(VERSION 3.16)
project(CLIENT C)

set(CMAKE_C_STANDARD 11)

add_executable(CLIENT client.c)

target_link_libraries(CLIENT -pthread)