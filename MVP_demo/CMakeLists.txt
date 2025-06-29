cmake_minimum_required(VERSION 3.8)
project(AutonomousDrivingMVP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# 包含头文件目录
include_directories(include)

# 查找线程库
find_package(Threads REQUIRED)

# 源文件
set(SOURCES
    src/model/vehicle_model.cpp
    src/view/dashboard_view.cpp
    src/present/main_presenter.cpp
    src/main.cpp
)

# 创建可执行文件
add_executable(autonomous_mvp ${SOURCES})

# 链接线程库
target_link_libraries(autonomous_mvp Threads::Threads)

# 添加测试
enable_testing()
add_subdirectory(test)

# 编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(autonomous_mvp PRIVATE DEBUG_MODE)
endif()