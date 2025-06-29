# 简单的测试配置
add_executable(test_adaptor
    test_adaptor.cpp
    ../src/legacy_lidar.cpp
    ../src/lidar_adaptor.cpp
    ../src/modern_camera.cpp
)

target_include_directories(test_adaptor PRIVATE ../include)

add_test(NAME AdaptorTest COMMAND test_adaptor)