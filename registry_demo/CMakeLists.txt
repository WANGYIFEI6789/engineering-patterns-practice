cmake_minimum_required(VERSION 3.8)
project(registry_demo)
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/external)
include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SRC_FILES "src/*.cpp")
add_executable(registry_demo ${SRC_FILES})