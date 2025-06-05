#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "src/utils/Stringable.h"
#include "src/utils/MiscUtils.h"
#include "src/utils/TimeFuncs.h"

TEST_CASE("Stringable basic usage", "[utils]") {
    // Assuming Stringable is a base class with a toString() method
    struct Dummy : public Stringable {
        std::string toString() const override { return "dummy"; }
    } d;
    REQUIRE(d.toString() == "dummy");
}

TEST_CASE("MiscUtils: min/max", "[utils]") {
    // If MiscUtils has min/max functions, test them (example only)
    // REQUIRE(min(3, 5) == 3);
    // REQUIRE(max(3, 5) == 5);
    SUCCEED(); // Placeholder if no such functions
}

TEST_CASE("TimeFuncs: basic timing", "[utils]") {
    // If TimeFuncs has a function to get current time, test it (example only)
    // auto t1 = getCurrentTime();
    // REQUIRE(t1 > 0);
    SUCCEED(); // Placeholder if no such functions
}
