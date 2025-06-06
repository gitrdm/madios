// File: main.cpp
// Purpose: Entry point for the madios project. Handles input/output and program flow for grammar induction using the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.
//
// Usage: ./madios <filename> <eta> <alpha> <context_size> <coverage> [--json] [number_of_new_sequences]
//
// This file is a good starting point for understanding how the program operates.
//
// Major responsibilities:
//   - Parse and validate command-line arguments
//   - Read and validate input corpus files
//   - Run the ADIOS grammar induction algorithm
//   - Output results in human-readable or JSON format
//   - Provide robust error handling and clear diagnostics

#include "MiscUtils.h"
#include "RDSGraph.h"
#include "special.h"
#include "TimeFuncs.h"
#include "utils/json.hpp"

#include <sstream>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <iomanip>

using std::vector;
using std::pair;
using std::string;
using std::stringstream;
using std::ifstream;
using std::ios;
using std::cout;
using std::endl;

/**
 * @brief Entry point for the madios program.
 *
 * Handles argument parsing, input file reading, and program flow for grammar induction.
 * Provides robust error handling and clear output for both human and machine (JSON) consumption.
 *
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return int Exit code (0 for success, nonzero for error)
 */
int main(int argc, char *argv[])
{
    // Parse command-line arguments and detect JSON mode
    bool json_mode = false;
    int positional_argc = 0;
    for(int i = 1; i < argc; ++i) {
        if(std::string(argv[i]) == "--json") {
            json_mode = true;
            break;
        } else {
            positional_argc++;
        }
    }
    if(positional_argc < 5) {
        std::cerr << "Usage:" << std::endl;
        std::cerr << "madios <filename> <eta> <alpha> <context_size> <coverage> [--json] [number_of_new_sequences]" << std::endl;
        return 1;
    }
    // Defensive: Validate and open input file
    std::ifstream infile(argv[1]);
    if (!infile.good()) {
        std::cerr << "[main] Error: Cannot open input file '" << argv[1] << "'." << std::endl;
        return 2;
    }
    infile.close();
    // Defensive: Parse numeric arguments
    double eta = 0.0, alpha = 0.0, coverage = 0.0;
    int context_size = 0;
    try {
        eta = std::stod(argv[2]);
        alpha = std::stod(argv[3]);
        context_size = std::stoi(argv[4]);
        coverage = std::stod(argv[5]);
    } catch (const std::exception &e) {
        std::cerr << "[main] Error: Invalid numeric argument. " << e.what() << std::endl;
        return 3;
    }
    // Read input sequences (robust to plain or ADIOS-style input)
    vector<vector<string> > sequences = readSequencesFromFile(argv[1]);
    if (sequences.empty()) {
        std::cerr << "[main] Error: No sequences found in input file '" << argv[1] << "'." << std::endl;
        return 4;
    }
    // Build the initial graph
    RDSGraph testGraph(sequences);
    testGraph.setQuiet(json_mode); // Suppress verbose output if --json is set
    double startTime = getTime();
    testGraph.distill(ADIOSParams(eta, alpha, context_size, coverage));
    double endTime = getTime();
    // Output results in JSON or human-readable format
    if(json_mode) {
        nlohmann::json j;
        j["corpus"] = sequences;
        // Search paths
        std::vector<std::vector<std::string>> search_paths;
        for(const auto& path : testGraph.getPaths()) {
            std::vector<std::string> s;
            for(unsigned int idx : path) {
                s.push_back(testGraph.getNodeName(idx));
            }
            search_paths.push_back(s);
        }
        j["search_paths"] = search_paths;
        // Lexicon
        std::vector<nlohmann::json> lexicon;
        const auto& nodes = testGraph.getNodes();
        for(size_t i = 0; i < nodes.size(); ++i) {
            nlohmann::json node_j;
            node_j["id"] = i;
            node_j["type"] = nodes[i].type;
            node_j["string"] = testGraph.getNodeString(i);
            std::vector<unsigned int> parents;
            for(const auto& p : nodes[i].parents) parents.push_back(p.first);
            node_j["parents"] = parents;
            lexicon.push_back(node_j);
        }
        j["lexicon"] = lexicon;
        // Grammar (PCFG)
        std::stringstream grammar_ss;
        testGraph.convert2PCFG(grammar_ss);
        j["grammar"] = grammar_ss.str();
        j["timing"] = endTime - startTime;
        std::cout << std::setw(2) << j << std::endl;
        return 0;
    } else {
        cout << "eta = " << eta << endl;
        cout << "alpha = " << alpha << endl;
        cout << "contextSize = " << context_size << endl;
        cout << "overlapThreshold = " << coverage << endl;
        cout << "BEGIN CORPUS ----------" << endl;
        for(unsigned int i = 0; i < sequences.size(); i++) {
            for(unsigned int j = 0; j < sequences[i].size(); j++)
                cout << sequences[i][j] << " ";
            cout << endl;
        }
        cout << "END CORPUS ----------" << endl << endl << endl;
        cout << testGraph << endl;
        cout << "BEGIN DISTILLATION ----------" << endl;
        testGraph.distill(ADIOSParams(eta, alpha, context_size, coverage));
        cout << "END DISTILLATION ----------" << endl << endl;
        cout << testGraph << endl << endl;
        std::cout << endl << "Time elapsed: " << endTime - startTime << " seconds" << endl << endl << endl << endl;
        testGraph.convert2PCFG(std::cout);
    }
    // Optionally: generate new sequences if requested (not enabled by default)
    /*
    if(argc > 6)
        for(unsigned int i = 0; i < static_cast<unsigned int>(atoi(argv[6])); i++)
        {
            vector<string> sequence = testGraph.generate();
            for(unsigned int j = 0; j < sequence.size(); j++)
                std::cout << sequence[j] << " ";
            std::cout << endl;
        }
    */
    return 0;
}
