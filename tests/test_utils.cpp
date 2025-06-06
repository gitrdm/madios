#include "catch.hpp"
#include "utils/Stringable.h"
#include "utils/MiscUtils.h"
#include "utils/TimeFuncs.h"
#include <sstream>

TEST_CASE("Stringable basic usage", "[utils]") {
    // Assuming Stringable is a base class with a toString() method
    struct Dummy : public Stringable {
        std::string toString() const override { return "dummy"; }
    } d;
    REQUIRE(d.toString() == "dummy");
}

TEST_CASE("MiscUtils: tokenise", "[utils]") {
    std::string line = "  Foo Bar\tBaz  ";
    auto tokens = tokenise(line);
    REQUIRE(tokens.size() >= 2);
}

TEST_CASE("MiscUtils: uppercase", "[utils]") {
    REQUIRE(uppercase("abc") == "ABC");
}

TEST_CASE("MiscUtils: lowercase", "[utils]") {
    REQUIRE(lowercase("ABC") == "abc");
}

TEST_CASE("MiscUtils: trimSpaces", "[utils]") {
    REQUIRE(trimSpaces("  hello world  ") == "hello world");
}
TEST_CASE("MiscUtils: trimSpaces minimal crash test", "[utils]") {
    std::string s = "  a  ";
    REQUIRE(trimSpaces(s) == "a");
}
TEST_CASE("MiscUtils: trimSpaces empty string", "[utils]") {
    REQUIRE(trimSpaces("") == "");
}
TEST_CASE("MiscUtils: trimSpaces all spaces", "[utils]") {
    REQUIRE(trimSpaces("     ") == "");
}

TEST_CASE("TimeFuncs: getTime monotonicity, getSeedFromTime", "[utils]") {
    double t1 = getTime();
    double t2 = getTime();
    REQUIRE(t2 >= t1);
    unsigned int seed = getSeedFromTime();
    REQUIRE(seed > 0);
}
