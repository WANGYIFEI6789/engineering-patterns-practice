#include "logger/log_formatter.hpp"
#include <sstream>
#include <iomanip>

namespace duan{
    namespace logger{
        std::string LogFormatter::format(
            LogLevel level, 
            const std::string &message, 
            const std::string &file, 
            int line, 
            const std::string &function, 
            const std::chrono::system_clock::time_point &timestamp) const {
            
            std::ostringstream oss;
            oss << "[" << format_timestamp(timestamp) << "] "
                << "[" << log_level_to_string(level) << "] "
                << "[" << file << ":" << line << "] "
                << "[" << function << "] "
                << message;
            return oss.str();
        }

        std::string LogFormatter::format_timestamp(const std::chrono::system_clock::time_point &timestamp) const {
            auto time_t = std::chrono::system_clock::to_time_t(timestamp);
            auto local_time = *std::localtime(&time_t);
            std::ostringstream oss;
            oss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
            return oss.str();
        }
    }
}