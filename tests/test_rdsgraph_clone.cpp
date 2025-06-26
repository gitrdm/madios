#ifndef MADIOS_TESTING
#define MADIOS_TESTING
#endif

#include "RDSGraph.h"
#include <iostream>
#include <vector>
#include <string>
#include <cassert>

int main() {
    // Construct a graph with two sequences that differ in the middle
    std::vector<std::vector<std::string>> sequences = {
        {"a", "b", "c"},
        {"a", "d", "c"}
    };
    RDSGraph graph(sequences);

    // Set parameters to force generalisation and EC creation
    ADIOSParams params(0.5, 0.5, 3, 0.0);

    // Use the first path for generalisation
    const auto& paths = graph.getPaths();
    assert(!paths.empty());
    const SearchPath& path = paths[0];

    // Set verbose output to see what happens
    graph.setQuiet(false);

    // Call generalise, which should trigger the clone code path
    bool result = graph.testGeneralise(path, params);

    std::cout << "Generalise result: " << (result ? "success" : "failure") << std::endl;
    // Optionally, print the graph state
    std::cout << graph.toString() << std::endl;

    return 0;
}
