/**
 * @file main.cpp
 * @brief Entry point and CLI handler for the madios ADIOS grammar induction tool.
 *
 * This file contains the main() function and the CLI logic for running the ADIOS grammar induction algorithm.
 * It handles argument parsing, input/output, error handling, and program flow.
 *
 * Usage: ./madios <filename> <eta> <alpha> <context_size> <coverage> [--json] [--pcfg] [number_of_new_sequences]
 *
 * For more details, see the README and documentation for the ADIOS algorithm.
 */

#include "MiscUtils.h"
#include "RDSGraph.h"
#include "special.h"
#include "TimeFuncs.h"
#include "utils/json.hpp"
#include "../ext/CLI11.hpp"
#include "madios/Logger.h"
#include "madios/version.h"
#include "madios/BasicSymbol.h"

#include <sstream>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <iomanip>
#include <sys/resource.h>

using std::vector;
using std::pair;
using std::string;
using std::stringstream;
using std::ifstream;
using std::ios;
using std::cout;
using std::endl;

/**
 * @brief Run the CLI interface for the madios program.
 *
 * Handles argument parsing, input file reading, and program flow for grammar induction.
 * Provides robust error handling and clear output for both human and machine (JSON) consumption.
 *
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return int Exit code (0 for success, nonzero for error)
 */
int run_cli(int argc, char *argv[])
{
    // Log program start and version info
    madios::Logger::info("madios CLI started");
    madios::Logger::info(std::string("madios version: ") + MADIOS_VERSION + ", git commit: " + MADIOS_GIT_COMMIT + ", build date: " + __DATE__);
    std::ostringstream cli_args;
    for (int i = 0; i < argc; ++i) cli_args << argv[i] << " ";
    madios::Logger::info("CLI arguments: " + cli_args.str());

    // --- Argument parsing using CLI11 ---
    CLI::App app{"madios: ADIOS grammar induction"};

    std::string input_filename;
    double eta = 0.9;
    double alpha = 0.01;
    int context_size = 5;
    double coverage = 0.65;
    std::string output_filename;
    bool json_mode = false;
    bool pcfg_mode = false;
    int num_new_sequences = 0;
    bool verbose = false;
    bool quiet = false;

    // Positional arguments (required)
    app.add_option("input", input_filename, "Input corpus file")->required();
    app.add_option("eta", eta, "Divergence threshold (e.g., 0.9)")->required();
    app.add_option("alpha", alpha, "Significance threshold (e.g., 0.01)")->required();
    app.add_option("context_size", context_size, "Context window size (e.g., 5)")->required();
    app.add_option("coverage", coverage, "Coverage threshold (e.g., 0.65)")->required();
    // Optional positional
    app.add_option("number_of_new_sequences", num_new_sequences, "Number of new sequences to generate");

    // Named options
    app.add_option("-o,--output", output_filename, "Output file (default: stdout)");
    app.add_flag("--json", json_mode, "Output all results as JSON");
    app.add_flag("--pcfg", pcfg_mode, "Output only the learned grammar in PCFG format");
    app.add_flag("--verbose", verbose, "Enable verbose output");
    app.add_flag("--quiet", quiet, "Suppress all non-error output");

    try {
        CLI11_PARSE(app, argc, argv);
    } catch (const std::exception &e) {
        madios::Logger::error(std::string("Error parsing command line: ") + e.what());
        return 1;
    }

    madios::Logger::trace("Parsing CLI arguments");

    // Mutually exclusive: if both set, quiet wins
    if (quiet) verbose = false;

    // Simple logging utility for verbose mode
    auto log_info = [&](const std::string& msg) {
        if (verbose && !quiet) std::cout << msg << std::endl;
    };

    // --- Input file validation and reading ---
    log_info("[madios] Reading input file: " + input_filename);
    madios::Logger::trace("Opening input file: " + input_filename);
    std::ifstream infile(input_filename);
    if (!infile.good()) {
        std::cerr << "[main] Error: Cannot open input file '" << input_filename << "'." << std::endl;
        madios::Logger::error(std::string("Error opening input file: ") + input_filename);
        return 2;
    }
    infile.close();
    // Read input sequences (robust to plain or ADIOS-style input)
    log_info("[madios] Parsing sequences from file...");
    vector<vector<string> > sequences = readSequencesFromFile(input_filename);
    if (sequences.empty()) {
        std::cerr << "[main] Error: No sequences found in input file '" << input_filename << "'." << std::endl;
        return 4;
    }
    // --- Build the initial ADIOS graph ---
    log_info("[madios] Building initial graph...");
    RDSGraph testGraph(sequences);
    testGraph.setQuiet(json_mode || pcfg_mode || quiet); // Suppress verbose output if --json, --pcfg, or --quiet is set
    double startTime = getTime();
    // --- Run the ADIOS grammar induction algorithm ---
    log_info("[madios] Running distillation...");
    madios::Logger::trace("Running ADIOS grammar induction");
    testGraph.distill(ADIOSParams(eta, alpha, context_size, coverage));
    double endTime = getTime();
    log_info("[madios] Distillation complete. Time elapsed: " + std::to_string(endTime - startTime) + " seconds");
    // --- Output handling: JSON, PCFG, or human-readable ---
    std::ostream* out = &std::cout;
    std::ofstream outfile;
    if (!output_filename.empty()) {
        outfile.open(output_filename);
        if (!outfile.is_open()) {
            std::cerr << "[main] Error: Cannot open output file '" << output_filename << "'." << std::endl;
            return 5;
        }
        out = &outfile;
    }
    // Determine default output file if not specified
    if (output_filename.empty()) {
        if (json_mode) {
            output_filename = "output.json";
        } else if (pcfg_mode) {
            output_filename = "output.pcfg";
        }
    }
    // Output results in JSON, PCFG, or human-readable format
    if(json_mode) {
        nlohmann::json j;
        j["corpus"] = sequences;
        // --- Output search paths ---
        std::vector<std::vector<std::string>> search_paths;
        for(const auto& path : testGraph.getPaths()) {
            std::vector<std::string> s;
            for(unsigned int idx : path) {
                s.push_back(testGraph.getNodeName(idx));
            }
            search_paths.push_back(s);
        }
        j["search_paths"] = search_paths;
        // --- Output lexicon ---
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
        // --- Output grammar (PCFG) ---
        std::stringstream grammar_ss;
        testGraph.convert2PCFG(grammar_ss);
        j["grammar"] = grammar_ss.str();
        j["timing"] = endTime - startTime;
        (*out) << std::setw(2) << j << std::endl;
        return 0;
    } else if(pcfg_mode) {
        // Output only the learned grammar in PCFG format
        testGraph.convert2PCFG(*out);
        return 0;
    } else {
        // Human-readable output: parameters, corpus, and graph
        (*out) << "eta = " << eta << std::endl;
        (*out) << "alpha = " << alpha << std::endl;
        (*out) << "contextSize = " << context_size << std::endl;
        (*out) << "overlapThreshold = " << coverage << std::endl;
        (*out) << "BEGIN CORPUS ----------" << std::endl;
        for(unsigned int i = 0; i < sequences.size(); i++) {
            for(unsigned int j = 0; j < sequences[i].size(); j++)
                (*out) << sequences[i][j] << " ";
            (*out) << std::endl;
        }
        (*out) << "END CORPUS ----------" << std::endl << std::endl << std::endl;
        (*out) << testGraph << std::endl;
        (*out) << "BEGIN DISTILLATION ----------" << std::endl;
        testGraph.distill(ADIOSParams(eta, alpha, context_size, coverage));
        (*out) << "END DISTILLATION ----------" << std::endl << std::endl;
        (*out) << testGraph << std::endl << std::endl;
        (*out) << std::endl << "Time elapsed: " << endTime - startTime << " seconds" << std::endl << std::endl << std::endl << std::endl;
        testGraph.convert2PCFG(*out);
    }
    // --- Optionally: generate new sequences if requested ---
    if(num_new_sequences > 0) {
        for(int i = 0; i < num_new_sequences; ++i) {
            vector<string> sequence = testGraph.generate();
            for(const auto& token : sequence)
                std::cout << token << " ";
            std::cout << std::endl;
        }
    }
    // --- Log summary statistics and resource usage ---
    madios::Logger::info("Input file: " + input_filename);
    if (!output_filename.empty()) madios::Logger::info("Output file: " + output_filename);
    madios::Logger::info("Random seed: " + std::to_string(getDeterministicSeed()));
    madios::Logger::info("Summary: patterns found = " + std::to_string(testGraph.getPatternCount()) + ", rewiring ops = " + std::to_string(testGraph.getRewiringCount()) + ", final graph size = " + std::to_string(testGraph.getNodes().size()));
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    madios::Logger::info("Peak memory usage: " + std::to_string(usage.ru_maxrss) + " KB");
    madios::Logger::info("madios CLI finished");
    madios::Logger::trace("CLI execution complete");
    return 0;
}

/**
 * @brief Entry point for the madios program.
 *
 * Calls run_cli() to handle all CLI logic and program flow.
 *
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return int Exit code (0 for success, nonzero for error)
 */
int main(int argc, char *argv[])
{
    return run_cli(argc, argv);
}
