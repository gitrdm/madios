#include "catch.hpp"
#include "EquivalenceClass.h"
#include <vector>

TEST_CASE("EquivalenceClass: construction and add/has", "[equiv]") {
    EquivalenceClass ec;
    REQUIRE_FALSE(ec.has(42));
    REQUIRE(ec.add(42));
    REQUIRE(ec.has(42));
    REQUIRE_FALSE(ec.add(42)); // should not add duplicate
}

TEST_CASE("EquivalenceClass: computeOverlapEC", "[equiv]") {
    EquivalenceClass ec1, ec2;
    ec1.add(1); ec1.add(2); ec1.add(3);
    ec2.add(2); ec2.add(3); ec2.add(4);
    EquivalenceClass overlap = ec1.computeOverlapEC(ec2);
    REQUIRE(overlap.has(2));
    REQUIRE(overlap.has(3));
    REQUIRE_FALSE(overlap.has(1));
    REQUIRE_FALSE(overlap.has(4));
}

TEST_CASE("EquivalenceClass: toString and makeCopy", "[equiv]") {
    EquivalenceClass ec;
    ec.add(7); ec.add(8);
    std::string s = ec.toString();
    REQUIRE(s.find("7") != std::string::npos);
    REQUIRE(s.find("8") != std::string::npos);
    LexiconUnit* copy = ec.makeCopy();
    REQUIRE(copy != nullptr);
    delete copy;
}
