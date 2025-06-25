#include <unistd.h>
#include "catch.hpp"
#include "RDSGraph.h"
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <map>

TEST_CASE("PCFG output is in standard format and probabilities are normalized (corpus)", "[pcfg][output]") {
    // Read corpus from file (relative to build directory)
    std::ifstream infile;
    infile.open("../test/corpus.txt");
    if (!infile.is_open()) {
        infile.open("test/corpus.txt");
    }
    if (!infile.is_open()) {
        // Print debug info
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd))) {
            WARN("Current working directory: " << cwd);
        }
        WARN("Could not open corpus file at '../test/corpus.txt' or 'test/corpus.txt'");
    }
    REQUIRE(infile.is_open());
    std::vector<std::vector<std::string>> corpus;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            if (token == "*" || token == "#") continue;
            tokens.push_back(token);
        }
        if (!tokens.empty()) corpus.push_back(tokens);
    }
    infile.close();
    
    RDSGraph g(corpus);
    g.setQuiet(true);
    g.distill(ADIOSParams(0.9, 0.01, 2, 0.5));
    std::stringstream ss;
    g.convert2PCFG(ss);
    std::string out = ss.str();

    // Parse PCFG rules and collect probabilities by LHS
    std::regex rule_regex(R"((\w+) -> .+ \[([0-9.eE+-]+)\])");
    std::map<std::string, std::vector<double>> lhs_probs;
    size_t rule_count = 0;
    for (std::string line; std::getline(ss, line); ) {
        std::smatch m;
        if (std::regex_match(line, m, rule_regex)) {
            rule_count++;
            std::string lhs = m[1];
            double prob = std::stod(m[2]);
            lhs_probs[lhs].push_back(prob);
        }
    }
    REQUIRE(rule_count > 0); // At least one rule
    // Debug print: show all LHSs and their probabilities
    for (const auto& kv : lhs_probs) {
        std::ostringstream oss;
        oss << "LHS: " << kv.first << " | probs:";
        double sum = 0.0;
        for (double p : kv.second) { oss << " " << p; sum += p; }
        oss << " | sum: " << sum;
        WARN(oss.str());
        REQUIRE(sum == Approx(1.0).margin(1e-6));
    }
}
