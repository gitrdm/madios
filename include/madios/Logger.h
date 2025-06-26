#pragma once
#include <iostream>
#include <string>
#include <mutex>

namespace madios {

/**
 * @brief Simple thread-safe logger for tracing and debugging.
 *
 * Usage:
 *   madios::Logger::info("message");
 *   madios::Logger::warn("warning");
 *   madios::Logger::error("error");
 *   madios::Logger::trace("trace");
 */
class Logger {
public:
    enum class Level { TRACE, INFO, WARN, ERROR };

    static void setLevel(Level level);
    static void trace(const std::string& msg);
    static void info(const std::string& msg);
    static void warn(const std::string& msg);
    static void error(const std::string& msg);
private:
    static void log(Level level, const std::string& msg);
    static Level currentLevel;
    static std::mutex logMutex;
};

} // namespace madios
