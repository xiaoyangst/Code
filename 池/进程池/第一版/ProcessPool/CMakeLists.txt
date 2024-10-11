cmake_minimum_required(VERSION 3.16)
project(ProcessPool C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

aux_source_directory(. SRC)

add_executable(${PROJECT_NAME} main.c ${SRC})

target_link_libraries(${PROJECT_NAME} -pthread)