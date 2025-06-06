#include "catch.hpp"
#include "RDSGraph.h"
#include "utils/MiscUtils.h"
#include <vector>
#include <string>
#include <fstream>
#include <cstdio>

TEST_CASE("RDSGraph handles non-standard input (plain text, no * or #)", "[input][plain]") {
    // Prepare a temp file with the same content as test/test_madios.txt
    const char* temp_filename = "test_madios_tmp.txt";
    std::ofstream out(temp_filename);
    out << "a b c\n";
    out << "d e f\n";
    out << "g h i\n";
    out.close();

    // Use the same input logic as main.cpp
    std::vector<std::vector<std::string> > sequences = readSequencesFromFile(temp_filename);

    REQUIRE(sequences.size() == 3);
    REQUIRE(sequences[0] == std::vector<std::string>{"a", "b", "c"});
    REQUIRE(sequences[1] == std::vector<std::string>{"d", "e", "f"});
    REQUIRE(sequences[2] == std::vector<std::string>{"g", "h", "i"});

    // Construct RDSGraph and check it doesn't throw or crash
    RDSGraph g(sequences);
    REQUIRE(g.getPaths().size() == 3);
    // Clean up temp file
    std::remove(temp_filename);
}
