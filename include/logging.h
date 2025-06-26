#ifndef MADIOS_LOGGING_H
#define MADIOS_LOGGING_H

#include <iostream>
#include <string>

namespace madios {

/**
 * @enum LogLevel
 * @brief Logging levels for madios logging utilities.
 */
enum LogLevel {
    LOG_ERROR = 0, ///< Error messages only
    LOG_WARN = 1,  ///< Warnings and errors
    LOG_INFO = 2,  ///< Informational, warnings, and errors
    LOG_DEBUG = 3  ///< Debug, info, warnings, and errors
};

/**
 * @brief Global log level (can be set at runtime).
 */
extern LogLevel CURRENT_LOG_LEVEL;

/**
 * @brief Set the global log level.
 * @param level The log level to set.
 */
inline void setLogLevel(LogLevel level) {
    CURRENT_LOG_LEVEL = level;
}

/**
 * @brief Log a message at the given log level.
 * @param level The log level for the message.
 * @param msg The message to log.
 */
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

/**
 * @def LOG_ERROR(msg)
 * @brief Log an error message.
 * @param msg The message to log.
 */
/**
 * @def LOG_WARN(msg)
 * @brief Log a warning message.
 * @param msg The message to log.
 */
/**
 * @def LOG_INFO(msg)
 * @brief Log an informational message.
 * @param msg The message to log.
 */
/**
 * @def LOG_DEBUG(msg)
 * @brief Log a debug message.
 * @param msg The message to log.
 */
#define LOG_ERROR(msg) madios::log(madios::LOG_ERROR, msg)
#define LOG_WARN(msg)  madios::log(madios::LOG_WARN,  msg)
#define LOG_INFO(msg)  madios::log(madios::LOG_INFO,  msg)
#define LOG_DEBUG(msg) madios::log(madios::LOG_DEBUG, msg)

} // namespace madios

#endif // MADIOS_LOGGING_H
