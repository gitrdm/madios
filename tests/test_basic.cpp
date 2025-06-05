#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("Basic arithmetic works", "[trivial]") {
    REQUIRE(1 + 1 == 2);
}
