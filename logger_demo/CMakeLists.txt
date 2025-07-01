cmake_minimum_required(VERSION 3.8)
project(LightweightLogger VERSION 1.0.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置编译选项
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# 包含头文件目录
include_directories(include)

# 源文件
set(LOGGER_SOURCES
    src/logger.cpp
    src/log_formatter.cpp
)

# 创建静态库
add_library(${PROJECT_NAME} STATIC ${LOGGER_SOURCES})

# 设置库的头文件路径
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 示例程序
add_executable(logger_example src/main.cpp)
target_link_libraries(logger_example ${PROJECT_NAME})

# 测试程序
add_executable(logger_test test/test_logger.cpp)
target_link_libraries(logger_test ${PROJECT_NAME})

# 安装规则
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)