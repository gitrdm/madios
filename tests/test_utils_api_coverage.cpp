// Test: Public API coverage for utility functions (example for MiscUtils)
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../utils/MiscUtils.h"

TEST_CASE("MiscUtils public functions work as expected", "[api][utils]") {
    // Example: test a utility function with normal and edge-case input
    REQUIRE(MiscUtils::trim("  test  ") == "test");
    REQUIRE(MiscUtils::trim("") == "");
    REQUIRE(MiscUtils::trim("   ") == "");
    // Add more cases for all public functions as needed
}
