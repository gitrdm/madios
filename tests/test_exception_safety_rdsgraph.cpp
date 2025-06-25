// Test: Exception safety for public API (example for RDSGraph)
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "RDSGraph.h"

TEST_CASE("RDSGraph public methods are exception safe", "[exception][rdsgraph]") {
    RDSGraph g;
    // Example: test that calling a method with invalid input throws or handles gracefully
    // Replace with actual method names and invalid input as appropriate
    // REQUIRE_NOTHROW(g.somePublicMethodThatShouldHandleErrorsGracefully());
    // For demonstration, check for null/empty input handling if such methods exist
    // Uncomment and adapt as needed:
    // REQUIRE_NOTHROW(g.loadFromFile("nonexistent_file.txt"));
}
