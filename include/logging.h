#ifndef MADIOS_LOGGING_H
#define MADIOS_LOGGING_H

#include <iostream>
#include <string>

namespace madios {

// Logging levels
enum LogLevel {
    LOG_ERROR = 0,
    LOG_WARN = 1,
    LOG_INFO = 2,
    LOG_DEBUG = 3
};

// Global log level (can be set at runtime)
extern LogLevel CURRENT_LOG_LEVEL;

inline void setLogLevel(LogLevel level) {
    CURRENT_LOG_LEVEL = level;
}

inline void log(LogLevel level, const std::string& msg) {
    if (level <= CURRENT_LOG_LEVEL) {
        switch (level) {
            case LOG_ERROR: std::cerr << "[ERROR] "; break;
            case LOG_WARN:  std::cerr << "[WARN]  "; break;
            case LOG_INFO:  std::cout << "[INFO]  "; break;
            case LOG_DEBUG: std::cout << "[DEBUG] "; break;
        }
        std::cout << msg << std::endl;
    }
}

// Convenience macros
#define LOG_ERROR(msg) madios::log(madios::LOG_ERROR, msg)
#define LOG_WARN(msg)  madios::log(madios::LOG_WARN,  msg)
#define LOG_INFO(msg)  madios::log(madios::LOG_INFO,  msg)
#define LOG_DEBUG(msg) madios::log(madios::LOG_DEBUG, msg)

} // namespace madios

#endif // MADIOS_LOGGING_H
