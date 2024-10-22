cmake_minimum_required(VERSION 3.16)
project(SheepChatServer)

set(CMAKE_CXX_STANDARD 20)

# 设置 Boost 查找选项
set(Boost_USE_STATIC_LIBS ON)         # 只查找静态库（Boost 的静态版本）
set(Boost_USE_DEBUG_LIBS OFF)         # 忽略调试版本的库
set(Boost_USE_RELEASE_LIBS ON)        # 只查找发布版本的库
set(Boost_USE_MULTITHREADED ON)       # 启用多线程支持
set(Boost_USE_STATIC_RUNTIME OFF)     # 使用动态运行时库（不强制静态运行时）

# 查找指定的 Boost 组件：system 和 filesystem
find_package(Boost REQUIRED COMPONENTS system filesystem)

# 如果找到了 Boost 库
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})  # 包含 Boost 的头文件目录

    include_directories(include)
    include_directories(third_party)

    aux_source_directory(src SRC)

    add_executable(SheepChatServer main.cpp ${SRC})

    target_link_libraries(SheepChatServer ${Boost_LIBRARIES} pthread)
endif()
