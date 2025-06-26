#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "madios/BasicSymbol.h"

TEST_CASE("Basic arithmetic works", "[trivial]") {
    REQUIRE(1 + 1 == 2);
}
