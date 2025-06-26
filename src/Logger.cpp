#include "madios/Logger.h"
#include "madios/BasicSymbol.h"
#include <iostream>
#include <chrono>
#include <ctime>

namespace madios {

Logger::Level Logger::currentLevel = Logger::Level::INFO;
std::mutex Logger::logMutex;

void Logger::setLevel(Level level) {
    currentLevel = level;
}

void Logger::trace(const std::string& msg) {
    log(Level::TRACE, msg);
}
void Logger::info(const std::string& msg) {
    log(Level::INFO, msg);
}
void Logger::warn(const std::string& msg) {
    log(Level::WARN, msg);
}
void Logger::error(const std::string& msg) {
    log(Level::ERROR, msg);
}

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
