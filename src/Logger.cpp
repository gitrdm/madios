// File: Logger.cpp
// Purpose: Implements the madios::Logger class for thread-safe logging with multiple log levels.
// Part of the ADIOS grammar induction project. See README for usage and structure.
//
// Major responsibilities:
//   - Provide thread-safe logging utilities for trace, info, warning, and error messages
//   - Support log level filtering and timestamped output
//
// Design notes:
//   - Uses a static mutex for thread safety
//   - All log output is timestamped and level-tagged
//   - Log level can be set globally

#include "madios/Logger.h"
#include "madios/BasicSymbol.h"
#include <iostream>
#include <chrono>
#include <ctime>

namespace madios {

Logger::Level Logger::currentLevel = Logger::Level::INFO;
std::mutex Logger::logMutex;

/**
 * @brief Set the global log level for Logger output.
 * @param level The minimum log level to display
 */
void Logger::setLevel(Level level) {
    currentLevel = level;
}

/**
 * @brief Log a trace-level message (for detailed debugging).
 * @param msg The message to log
 */
void Logger::trace(const std::string& msg) {
    log(Level::TRACE, msg);
}
/**
 * @brief Log an info-level message (for general information).
 * @param msg The message to log
 */
void Logger::info(const std::string& msg) {
    log(Level::INFO, msg);
}
/**
 * @brief Log a warning-level message.
 * @param msg The message to log
 */
void Logger::warn(const std::string& msg) {
    log(Level::WARN, msg);
}
/**
 * @brief Log an error-level message.
 * @param msg The message to log
 */
void Logger::error(const std::string& msg) {
    log(Level::ERROR, msg);
}

/**
 * @brief Internal log function. Outputs timestamped, level-tagged messages if level >= currentLevel.
 * @param level The log level of the message
 * @param msg The message to log
 */
void Logger::log(Level level, const std::string& msg) {
    if (level < currentLevel) return;
    std::lock_guard<std::mutex> lock(logMutex);
    const char* levelStr = "INFO";
    switch (level) {
        case Level::TRACE: levelStr = "TRACE"; break;
        case Level::INFO:  levelStr = "INFO";  break;
        case Level::WARN:  levelStr = "WARN";  break;
        case Level::ERROR: levelStr = "ERROR"; break;
    }
    // Timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm_buf;
#if defined(_MSC_VER)
    localtime_s(&tm_buf, &now_c);
    std::tm* tm = &tm_buf;
#else
    std::tm* tm = std::localtime(&now_c);
#endif
    char timebuf[20];
    std::strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm);
    std::cerr << "[" << timebuf << "] [" << levelStr << "] " << msg << std::endl;
}

} // namespace madios
