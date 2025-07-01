#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <sstream>
#include "log_level.hpp"
#include "log_formatter.hpp"

namespace duan
{
    namespace logger{
        class Logger{
        public:
            static Logger& instance();

            // 禁用拷贝构造和赋值
            Logger(const Logger&) = delete;
            Logger& operator=(const Logger&) = delete;

            // 配置方法
            void set_level(LogLevel level);
            void set_output_file(const std::string& filename);
            void enable_console_output(bool enable);
            void enable_file_output(bool enable);
            void set_formatter(std::unique_ptr<LogFormatter> formatter);
            
            // 核心日志方法
            void log(LogLevel level, 
                    const std::string& message, 
                    const std::string& file, 
                    int line, 
                    const std::string& function);
            
            // 模板方法支持格式化
            template<typename... Args>
            void log_formatter(
                LogLevel level, 
                const std::string& file, 
                int line, 
                const std::string& function, 
                const std::string& format_str, 
                Args&&... args);
            
            // 获取当前日志级别
            LogLevel get_level() const { return current_level_;}
        
        private:
            Logger();
            ~Logger();

            void write_to_console(const std::string& formatted_message);
            void write_to_file(const std::string& formatted_message);

            template<typename T>
            std::string format_arg(T&& arg);

            std::string format_string(const std::string& format_str);
            
            template<typename T, typename... Args>
            std::string format_string(const std::string& format_str, T&& arg, Args&&... args);

        private:
            LogLevel current_level_{LogLevel::INFO};
            bool console_output_enabled_{true};
            bool file_output_enabled_{false};

            std::unique_ptr<LogFormatter> formatter_;
            std::string log_filename_;
            std::ofstream log_file_;

            std::mutex log_mutex_; // 保护日志写入的互斥锁
        };


        // 模板方法实现
        template<typename... Args>
        void Logger::log_formatter(LogLevel level, 
                                   const std::string& file, 
                                   int line, 
                                   const std::string& function, 
                                   const std::string& format_str, 
                                   Args&&... args) {
            if (level < current_level_) {
                return; // 如果日志级别低于当前设置的级别，则不记录
            }

            // 格式化字符串
            std::string formatted_message = format_string(format_str, std::forward<Args>(args)...);       
            log(level, formatted_message, file, line, function);
        }

        template<typename T>
        std::string Logger::format_arg(T&& arg) {
            std::ostringstream oss;
            oss << arg;
            return oss.str();
        }

        template<typename T, typename... Args>
        std::string Logger::format_string(const std::string& format_str, T&& arg, Args&&... args) {
            size_t pos = format_str.find("{}");
            if (pos == std::string::npos) {
                return format_str; // 如果没有占位符，直接返回原字符串
            }

            std::string result = format_str.substr(0, pos) + 
                                format_arg(std::forward<T>(arg)) + 
                                format_str.substr(pos + 2);
            if constexpr (sizeof...(args) > 0) {
                return format_string(result, std::forward<Args>(args)...);
            } else {
                return result; // 如果没有更多参数，返回结果
            }
        }
    }
}

// 宏定义
#define DUAN_LOG_DEBUG(msg, ...) \
    duan::logger::Logger::instance().log_formatter(duan::logger::LogLevel::DEBUG, __FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__)

#define DUAN_LOG_INFO(msg, ...) \
    duan::logger::Logger::instance().log_formatter(duan::logger::LogLevel::INFO, __FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__)

#define DUAN_LOG_WARN(msg, ...) \
    duan::logger::Logger::instance().log_formatter(duan::logger::LogLevel::WARN, __FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__)

#define DUAN_LOG_ERROR(msg, ...) \
    duan::logger::Logger::instance().log_formatter(duan::logger::LogLevel::ERROR, __FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__)

#define DUAN_LOG_FATAL(msg, ...) \
    duan::logger::Logger::instance().log_formatter(duan::logger::LogLevel::FATAL, __FILE__, __LINE__, __FUNCTION__, msg, ##__VA_ARGS__)