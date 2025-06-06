#include "catch.hpp"
#include "RDSGraph.h"
#include "utils/json.hpp"
#include <vector>
#include <string>

TEST_CASE("RDSGraph: JSON output structure", "[rdsgraph][json]") {
    std::vector<std::vector<std::string>> corpus = {
        {"A", "B", "C"},
        {"A", "C", "B"}
    };
    RDSGraph g(corpus);
    g.setQuiet(true);
    g.distill(ADIOSParams(0.9, 0.01, 3, 0.5));
    // Simulate JSON output (as in main.cpp)
    nlohmann::json j;
    j["corpus"] = corpus;
    j["search_paths"] = g.getPaths();
    j["lexicon"] = nlohmann::json::array();
    for (size_t i = 0; i < g.getNodes().size(); ++i) {
        const auto& node = g.getNodes()[i];
        nlohmann::json n;
        n["id"] = static_cast<int>(i);
        n["string"] = node.lexicon ? node.lexicon->toString() : "";
        n["type"] = node.type;
        n["parents"] = nlohmann::json::array();
        for (const auto& p : node.parents) n["parents"].push_back(p.first);
        j["lexicon"].push_back(n);
    }
    REQUIRE(j.contains("corpus"));
    REQUIRE(j.contains("search_paths"));
    REQUIRE(j.contains("lexicon"));
}
