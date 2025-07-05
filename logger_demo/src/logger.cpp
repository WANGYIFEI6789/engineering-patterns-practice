#include "logger/logger.hpp"
#include <iostream>
#include <iomanip>

namespace duan
{
    namespace logger{
        const char* log_level_to_string(LogLevel level) {
            switch (level) {
                case LogLevel::DEBUG: return "DEBUG";
                case LogLevel::INFO: return "INFO";
                case LogLevel::WARN: return "WARN";
                case LogLevel::ERROR: return "ERROR";
                case LogLevel::FATAL: return "FATAL";
                default: return "UNKNOWN";
            }
        }

        LogLevel string_to_log_level(const std::string& level_str) {
            if (level_str == "DEBUG") return LogLevel::DEBUG;
            if (level_str == "INFO") return LogLevel::INFO;
            if (level_str == "WARN") return LogLevel::WARN;
            if (level_str == "ERROR") return LogLevel::ERROR;
            if (level_str == "FATAL") return LogLevel::FATAL;
            return LogLevel::INFO; // 默认级别
        }

        Logger& Logger::instance() {
            static Logger instance;
            return instance;
        }

        Logger::Logger() {
            // 默认日志格式化器
            formatter_ = std::make_unique<LogFormatter>();
        }

        Logger::~Logger() {
            if (log_file_.is_open()) {
                log_file_.close();
            }
        }

        void Logger::set_level(LogLevel level) {
            std::lock_guard<std::mutex> lock(log_mutex_);
            current_level_ = level;
        }

        void Logger::set_output_file(const std::string& filename) {
            std::lock_guard<std::mutex> lock(log_mutex_);
            log_filename_ = filename;
            if (log_file_.is_open()) {
                log_file_.close();
            }

            log_filename_ = filename;
            log_file_.open(log_filename_, std::ios::app);   // std::ios::app 追加模式
            if(!log_file_.is_open()) {
                std::cerr << "Failed to open log file: " << log_filename_ << std::endl;
            } else {
                file_output_enabled_ = true;
            }
        }

        void Logger::enable_console_output(bool enable) {
            std::lock_guard<std::mutex> lock(log_mutex_);
            console_output_enabled_ = enable;
        }

        void Logger::enable_file_output(bool enable) {
            std::lock_guard<std::mutex> lock(log_mutex_);
            file_output_enabled_ = enable;
        }

        void Logger::set_formatter(std::unique_ptr<LogFormatter> formatter) {
            std::lock_guard<std::mutex> lock(log_mutex_);
            formatter_ = std::move(formatter);
        }

        void Logger::log(LogLevel level, 
                         const std::string& message, 
                         const std::string& file, 
                         int line, 
                         const std::string& function) {
            if (level < current_level_) {
                return; // 如果日志级别低于当前设置的级别，则不记录
            }

            auto now = std::chrono::system_clock::now();
            // 使用格式化器格式化日志消息
            std::string formatted_message = formatter_->format(level, message, file, line, function, now);

            std::lock_guard<std::mutex> lock(log_mutex_);
            if (console_output_enabled_) {
                write_to_console(formatted_message);
            }
            if (file_output_enabled_ && log_file_.is_open()) {
                write_to_file(formatted_message);
            }
        }

        void Logger::write_to_console(const std::string& formatted_message) {
            std::cout << formatted_message << std::endl;
        }

        void Logger::write_to_file(const std::string& formatted_message) {
            if (log_file_.is_open()) {
                log_file_ << formatted_message << std::endl;
                log_file_.flush(); // 确保数据被写入文件
            }
        }

        std::string Logger::format_string(const std::string& format_str) {
            return format_str; // 简单返回原字符串
        }
    }
} 













/*
log_level 是日志等级
log_formatter 是日志格式化器
logger 是日志记录器
logger中有一个log_formatter成员变量，类型是std::unique_ptr<LogFormatter>
log_formatter中有一个format方法，用于格式化日志消息
log_formatter的format方法接受日志级别、消息、文件名、行号、函数名和时间戳作为参数

logger的log方法调用log_formatter的format方法来格式化日志消息
logger的log方法还会根据当前日志级别决定是否记录日志
logger的log方法会将格式化后的日志消息输出到控制台和文件（如果启用文件输出）
logger的log方法使用互斥锁来保护日志写入操作

logger.log_formatter 调用log接口  log接口中通过log_formatter对象的format方法来格式化日志消息
*/