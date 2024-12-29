cmake_minimum_required(VERSION 3.16)
project(Study)

set(CMAKE_CXX_STANDARD 17)
aux_source_directory(. SRC)
add_executable(Study main.cpp ${SRC})
target_link_libraries(Study tinyxml2)