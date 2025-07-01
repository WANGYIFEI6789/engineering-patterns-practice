#include "logger/logger.hpp"
#include <thread>
#include <chrono>
#include <vector>

int main(){
    // 配置日志系统
    duan::logger::Logger::instance().set_level(duan::logger::LogLevel::DEBUG);
    duan::logger::Logger::instance().set_output_file("duan_log.log");
    duan::logger::Logger::instance().enable_console_output(true);
    duan::logger::Logger::instance().enable_file_output(true);

    // 基本日志使用
    DUAN_LOG_INFO("This is an info message.");
    
    // 带参数的格式化日志
    std::string user = "duan";
    int age = 24;
    double salary = 8000.00;
    DUAN_LOG_INFO("User: {}, Age: {}, Salary: {}", user, age, salary);

    // 模拟多线程日志
    std::vector<std::thread> threads;
    for(int i = 0; i < 5; ++i){
        threads.emplace_back([i](){
            for(int j = 0; j < 5; ++j){
                DUAN_LOG_INFO("Thread {}: Log message {}", i, j);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }

    // 等待所有线程完成
    for(auto& t : threads){
        t.join();
    }

    return 0;
}