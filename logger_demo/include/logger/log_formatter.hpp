#pragma once

#include <string>
#include <chrono>
#include "log_level.hpp"

namespace duan {
    namespace logger{
        class LogFormatter{
        public:
            LogFormatter() = default;
            virtual ~LogFormatter() = default;

            // 格式化日志消息
            virtual std::string format(
                LogLevel level, 
                const std::string &message, 
                const std::string &file, 
                int line, 
                const std::string &function, 
                const std::chrono::system_clock::time_point &timestamp
            ) const;

        private:
            std::string format_timestamp(const std::chrono::system_clock::time_point &timestamp) const;
        };
    }
}