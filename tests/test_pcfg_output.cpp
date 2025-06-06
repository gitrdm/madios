#include "catch.hpp"
#include "RDSGraph.h"
#include <sstream>
#include <vector>
#include <string>
#include <regex>

TEST_CASE("PCFG output is in standard format and probabilities are normalized", "[pcfg][output]") {
    // Small test corpus
    std::vector<std::vector<std::string>> corpus = {
        {"A", "B"},
        {"A", "C"},
        {"A", "B"}
    };
    RDSGraph g(corpus);
    g.setQuiet(true);
    g.distill(ADIOSParams(0.9, 0.01, 2, 0.5));
    std::stringstream ss;
    g.convert2PCFG(ss);
    std::string out = ss.str();

    // Check for standard PCFG rule format: LHS -> RHS [prob]
    std::regex rule_regex(R"((\w+) -> .+ \[[0-9.eE+-]+\])");
    size_t rule_count = 0;
    double sum_e0 = 0.0;
    for (std::string line; std::getline(ss, line); ) {
        std::smatch m;
        if (std::regex_match(line, m, rule_regex)) {
            rule_count++;
            // Optionally, check normalization for a specific LHS (e.g., E0)
            if (m[1] == "E0") {
                size_t lbracket = line.find("[");
                size_t rbracket = line.find("]");
                if (lbracket != std::string::npos && rbracket != std::string::npos) {
                    double prob = std::stod(line.substr(lbracket+1, rbracket-lbracket-1));
                    sum_e0 += prob;
                }
            }
        }
    }
    REQUIRE(rule_count > 0); // At least one rule
    // Probabilities for E0 should sum to (approximately) 1
    REQUIRE(sum_e0 == Approx(1.0).margin(1e-6));
}
