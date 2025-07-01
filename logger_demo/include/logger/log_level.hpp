#pragma once
#include <string>

namespace duan{
namespace logger {
        enum class LogLevel{
            DEBUG = 0,
            INFO = 1,
            WARN = 2,
            ERROR = 3,
            FATAL = 4,
        };

        const char* log_level_to_string(LogLevel level);
        LogLevel string_to_log_level(const std::string& level_str);
    }
}