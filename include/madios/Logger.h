/**
 * @file Logger.h
 * @brief Declares the madios::Logger class for thread-safe logging.
 *
 * Provides static methods for logging messages at various severity levels.
 */
#pragma once
#include <iostream>
#include <string>
#include <mutex>

namespace madios {

/**
 * @class Logger
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
    /**
     * @enum Level
     * @brief Logging severity levels.
     */
    enum class Level { TRACE, INFO, WARN, ERROR };

    /**
     * @brief Set the current logging level.
     * @param level The minimum level to log.
     */
    static void setLevel(Level level);
    /**
     * @brief Log a trace-level message.
     * @param msg The message to log.
     */
    static void trace(const std::string& msg);
    /**
     * @brief Log an info-level message.
     * @param msg The message to log.
     */
    static void info(const std::string& msg);
    /**
     * @brief Log a warning-level message.
     * @param msg The message to log.
     */
    static void warn(const std::string& msg);
    /**
     * @brief Log an error-level message.
     * @param msg The message to log.
     */
    static void error(const std::string& msg);
private:
    /**
     * @brief Internal logging implementation.
     * @param level The severity level.
     * @param msg The message to log.
     */
    static void log(Level level, const std::string& msg);
    static Level currentLevel; ///< Current logging level.
    static std::mutex logMutex; ///< Mutex for thread safety.
};

} // namespace madios
