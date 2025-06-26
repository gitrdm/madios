#include "catch.hpp"
#include "madios/BasicSymbol.h"
#include "RDSNode.h"
#include "RDSGraph.h"

TEST_CASE("BasicSymbol: construction and equality", "[core]") {
    BasicSymbol a("foo");
    BasicSymbol b("foo");
    BasicSymbol c("bar");
    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}

TEST_CASE("RDSNode: construction", "[core]") {
    auto sym = std::make_unique<BasicSymbol>("test");
    RDSNode node(std::move(sym), LexiconTypes::Symbol);
    REQUIRE(node.lexicon->toString() == "test");
    REQUIRE(node.type == LexiconTypes::Symbol);
}

TEST_CASE("RDSGraph: construction", "[core]") {
    RDSGraph g;
    // Check that nodes and paths are empty on construction
    REQUIRE(g.getNodes().size() == 0);
    REQUIRE(g.getPaths().size() == 0);
}

TEST_CASE("RDSNode: addConnection and addParent", "[core]") {
    RDSNode node(std::make_unique<BasicSymbol>("foo"), LexiconTypes::Symbol);
    Connection c1 = {1, 2};
    node.addConnection(c1);
    REQUIRE(node.getConnections().size() == 1);
    REQUIRE(node.addParent(c1));
    REQUIRE_FALSE(node.addParent(c1)); // duplicate parent
}

TEST_CASE("RDSNode: deep copy", "[core]") {
    RDSNode node1(std::make_unique<BasicSymbol>("bar"), LexiconTypes::Symbol);
    node1.addConnection({3, 4});
    node1.addParent({5, 6});
    RDSNode node2 = node1;
    REQUIRE(node2.getConnections().size() == 1);
    REQUIRE(node2.parents.size() == 1);
    REQUIRE(node2.lexicon->toString() == "bar");
}
