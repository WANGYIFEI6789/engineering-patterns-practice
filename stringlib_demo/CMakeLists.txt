cmake_minimum_required(VERSION 3.8)
project(strutil)

set(CMAKE_CXX_STANDARD 17)

# 让include/ 作为头文件查找路径
include_directories(${CMAKE_SOURCE_DIR}/include)

# 编译动态库
add_library(strutil SHARED src/strutil.cpp)

set_target_properties(strutil PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

# 安装支持（可选，方便未来 make install）
install(TARGETS strutil LIBRARY DESTINATION lib)
install(FILES include/strutil.hpp DESTINATION include)