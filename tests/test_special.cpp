#include "catch.hpp"
#include "maths/special.h"
#include <cmath>

TEST_CASE("special: uniform_rand and normal_rand", "[special]") {
    double u = uniform_rand();
    REQUIRE(u >= 0.0);
    REQUIRE(u <= 1.0);
    double n = normal_rand();
    // Not much to check, but should be finite
    REQUIRE(std::isfinite(n));
}

TEST_CASE("special: gammaln, digamma, factln, binom", "[special]") {
    REQUIRE(std::abs(gammaln(1.0)) < 1e-8);
    REQUIRE(std::abs(digamma(1.0) + 0.5772) < 1e-2); // Euler-Mascheroni
    REQUIRE(std::abs(factln(5) - std::log(120.0)) < 1e-8);
    REQUIRE(std::abs(binom(2, 4, 0.5) - 0.375) < 1e-8);
}

TEST_CASE("special: solve_cubic real roots", "[special]") {
    double r0, r1, r2;
    // x^3 - 6x^2 + 11x - 6 = 0 has roots 1,2,3
    unsigned int nroots = solve_cubic(1, -6, 11, -6, r0, r1, r2);
    REQUIRE(nroots == 3);
    REQUIRE((std::abs(r0-3)<1e-6 || std::abs(r1-3)<1e-6 || std::abs(r2-3)<1e-6));
    REQUIRE((std::abs(r0-2)<1e-6 || std::abs(r1-2)<1e-6 || std::abs(r2-2)<1e-6));
    REQUIRE((std::abs(r0-1)<1e-6 || std::abs(r1-1)<1e-6 || std::abs(r2-1)<1e-6));
}
