// File: test_core.cpp
// Purpose: Core unit tests for madios data structures and logic.
//
// This file tests the construction, equality, and basic operations of
// core classes such as BasicSymbol, RDSNode, and RDSGraph. It ensures
// that the most fundamental building blocks of the project behave as expected.
//
// Each test case checks a specific aspect of the core logic.

#include "catch.hpp"
#include "madios/BasicSymbol.h"
#include "RDSNode.h"
#include "RDSGraph.h"

// Test construction and equality of BasicSymbol
TEST_CASE("BasicSymbol: construction and equality", "[core]") {
    BasicSymbol a("foo");
    BasicSymbol b("foo");
    BasicSymbol c("bar");
    REQUIRE(a == b);
    REQUIRE_FALSE(a == c);
}

// Test construction of RDSNode
TEST_CASE("RDSNode: construction", "[core]") {
    auto sym = std::make_unique<BasicSymbol>("test");
    RDSNode node(std::move(sym), LexiconTypes::Symbol);
    REQUIRE(node.lexicon->toString() == "test");
    REQUIRE(node.type == LexiconTypes::Symbol);
}

// Test construction of RDSGraph
TEST_CASE("RDSGraph: construction", "[core]") {
    RDSGraph g;
    // Check that nodes and paths are empty on construction
    REQUIRE(g.getNodes().size() == 0);
    REQUIRE(g.getPaths().size() == 0);
}

// Test addConnection and addParent for RDSNode
TEST_CASE("RDSNode: addConnection and addParent", "[core]") {
    RDSNode node(std::make_unique<BasicSymbol>("foo"), LexiconTypes::Symbol);
    Connection c1 = {1, 2};
    node.addConnection(c1);
    REQUIRE(node.getConnections().size() == 1);
    REQUIRE(node.addParent(c1));
    REQUIRE_FALSE(node.addParent(c1)); // duplicate parent
}

// Test deep copy of RDSNode
TEST_CASE("RDSNode: deep copy", "[core]") {
    RDSNode node1(std::make_unique<BasicSymbol>("bar"), LexiconTypes::Symbol);
    node1.addConnection({3, 4});
    node1.addParent({5, 6});
    RDSNode node2 = node1;
    REQUIRE(node2.getConnections().size() == 1);
    REQUIRE(node2.parents.size() == 1);
    REQUIRE(node2.lexicon->toString() == "bar");
}

// Test error handling for RDSNode (invalid input)
TEST_CASE("RDSNode: error handling for invalid input", "[core]") {
    // Test construction with null lexicon pointer
    REQUIRE_THROWS_AS(RDSNode(nullptr, LexiconTypes::Symbol), std::invalid_argument);
    // Test addConnection with invalid indices
    RDSNode node(std::make_unique<BasicSymbol>("err"), LexiconTypes::Symbol);
    Connection bad = {static_cast<unsigned int>(-1), 2};
    REQUIRE_THROWS_AS(node.addConnection(bad), std::invalid_argument);
    // Test addParent with invalid indices
    Connection bad_parent = {1, static_cast<unsigned int>(-1)};
    REQUIRE_THROWS_AS(node.addParent(bad_parent), std::invalid_argument);
}
