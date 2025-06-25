// Test: Error handling for bad CLI arguments
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <cstdlib>

TEST_CASE("Bad CLI arguments trigger error handling", "[error][cli]") {
    int result = std::system("./madios --nonexistentflag > /dev/null 2>&1");
    REQUIRE(result != 0);
}

TEST_CASE("Missing required CLI argument triggers error handling", "[error][cli][missing]") {
    int result = std::system("./madios > /dev/null 2>&1");
    REQUIRE(result != 0);
}
