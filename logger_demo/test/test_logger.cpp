#include "logger/logger.hpp"
#include <cassert>
#include <sstream>
#include <fstream>
#include <iostream>

void test_basic_logging(){
    DUAN_LOG_INFO("基本日志测试");
    DUAN_LOG_DEBUG("调试信息");
    DUAN_LOG_WARN("警告信息");
    DUAN_LOG_ERROR("错误信息");

    std::cout << "基本日志测试完成" << std::endl;
}

void test_formatted_logging(){
    std::string user = "test_user";
    int value = 520;
    double pi = 3.14159;

    DUAN_LOG_INFO("name: {}, value: {}, pi: {}", user, value, pi);

    std::cout << "格式化日志测试完成" << std::endl;
}

void test_log_levels(){
    // 设置日志级别为 WARN
    duan::logger::Logger::instance().set_level(duan::logger::LogLevel::WARN);

    DUAN_LOG_DEBUG("调试信息");  // 这条日志不会被记录
    DUAN_LOG_INFO("信息日志");   // 这条日志不会被记录
    DUAN_LOG_WARN("警告日志");   // 这条日志会被记录
    DUAN_LOG_ERROR("错误日志");  // 这条日志会被记录

    std::cout << "日志级别测试完成" << std::endl;
}

void test_file_output(){
    // 设置日志输出到文件
    duan::logger::Logger::instance().set_output_file("test_log.log");
    DUAN_LOG_INFO("文件输出测试开始");

    for(int i = 0; i < 5; ++i){
        DUAN_LOG_INFO("这是第 {} 条日志", i + 1);
    }

    std::cout << "文件输出测试完成，日志已写入 test_log.log" << std::endl;
}

int main(){
    // 测试基本日志功能
    test_basic_logging();

    // 测试格式化日志
    test_formatted_logging();

    // 测试日志级别
    test_log_levels();

    // 测试文件输出
    test_file_output();

    std::cout << "所有测试完成" << std::endl;
    return 0;
}