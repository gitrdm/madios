// File: test_basic.cpp
// Purpose: Basic sanity check for the test framework and build system.
// This test ensures that the Catch2 test framework is working and that
// the build system is correctly compiling and running tests.
//
// This is a good starting point for new contributors to verify their setup.

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "madios/BasicSymbol.h"

// This test case checks that basic arithmetic works as expected.
TEST_CASE("Basic arithmetic works", "[trivial]") {
    REQUIRE(1 + 1 == 2);
}
