cmake_minimum_required(VERSION 3.8)
project(AdaptorPatternDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件目录
include_directories(include)

# 添加可执行文件
add_executable(adaptor_demo
    src/main.cpp
    src/legacy_lidar.cpp
    src/lidar_adaptor.cpp
    src/modern_camera.cpp
)

# 添加测试
enable_testing()
add_subdirectory(test)