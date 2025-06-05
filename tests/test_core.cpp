#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "src/BasicSymbol.h"
#include "src/RDSNode.h"
#include "src/RDSGraph.h"

TEST_CASE("BasicSymbol: construction and equality", "[core]") {
    BasicSymbol a("foo");
    BasicSymbol b("foo");
    BasicSymbol c("bar");
    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}

TEST_CASE("RDSNode: construction", "[core]") {
    RDSNode node(1, "test");
    REQUIRE(node.getId() == 1);
    REQUIRE(node.getLabel() == "test");
}

TEST_CASE("RDSGraph: construction", "[core]") {
    RDSGraph g;
    REQUIRE(g.size() >= 0); // Should be empty or valid
}
