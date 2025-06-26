#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "madios/Logger.h"
#include <sstream>

TEST_CASE("Logger basic usage", "[logger]") {
    madios::Logger::setLevel(madios::Logger::Level::TRACE);
    REQUIRE_NOTHROW(madios::Logger::trace("trace message"));
    REQUIRE_NOTHROW(madios::Logger::info("info message"));
    REQUIRE_NOTHROW(madios::Logger::warn("warn message"));
    REQUIRE_NOTHROW(madios::Logger::error("error message"));
}

TEST_CASE("Logger respects log level", "[logger]") {
    madios::Logger::setLevel(madios::Logger::Level::ERROR);
    // Only error should be output
    REQUIRE_NOTHROW(madios::Logger::trace("should not appear"));
    REQUIRE_NOTHROW(madios::Logger::info("should not appear"));
    REQUIRE_NOTHROW(madios::Logger::warn("should not appear"));
    REQUIRE_NOTHROW(madios::Logger::error("should appear"));
}
