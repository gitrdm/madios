// File: RDSGraph.cpp
// Purpose: Implements the RDSGraph class, which manages the main graph structure for pattern discovery in the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.
//
// Major responsibilities:
//   - Build and maintain the graph structure from input sequences
//   - Discover and generalize significant patterns
//   - Estimate probabilities for grammar induction
//   - Provide robust, defensive guards against memory errors
//   - Output and debug utilities for graph inspection
//
// Design notes:
//   - All public methods are robust to invalid input and out-of-bounds access
//   - Internal state is always kept consistent after any operation
//   - All output is guarded by the 'quiet' flag for flexible verbosity
//   - All major steps are documented inline for maintainability

// === Variable Naming Reference ===
// all_general_slots   // CORRECT
// some_patterns       // CORRECT
// DO NOT use: all_general_slots, some_patterns, etc.
//
// Add new variables here if naming is critical for refactor or LLM editing.

#include "RDSGraph.h"
#include "logging.h"
#include "utils/TimeFuncs.h"
#include "madios/maths/tnt/array2d.h"
#include "madios/Logger.h"
#include "madios/BasicSymbol.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <memory>

using std::min;
using std::max;
using std::vector;
using std::pair;
using std::string;
using std::ostream;
using std::ostringstream;
using std::endl;

// Utility: Check if a pattern is significant given p-values and alpha
inline bool isPatternSignificant(const SignificancePair &pvalues, double alpha)
{
    return (pvalues.first < alpha) && (pvalues.second < alpha);
}

// Utility: Comparison for significance pairs (used for sorting/selecting best patterns)
bool operator<(const SignificancePair &a, const SignificancePair &b)
{
    const double maxA = max(a.first, a.second);
    const double maxB = max(b.first, b.second);
    return maxA < maxB;
}

ADIOSParams::ADIOSParams(double eta, double alpha, unsigned int contextSize, double overlapThreshold)
{
    assert((eta >= 0.0) && (eta <= 1.0));
    assert((alpha >= 0.0) && (alpha <= 1.0));

    this->eta = eta;
    this->alpha = alpha;
    this->contextSize = contextSize;
    this->overlapThreshold = overlapThreshold;
}

/**
 * @brief Constructs an empty RDSGraph.
 */
RDSGraph::RDSGraph()
{
    corpusSize = 0;
}

/**
 * @brief Constructs an RDSGraph from a set of input sequences.
 * @param sequences A vector of input sequences (each sequence is a vector of strings).
 */
RDSGraph::RDSGraph(const vector<vector<string> > &sequences)
{
    srand(getSeedFromTime());
    buildInitialGraph(sequences);
}

/**
 * @brief Main distillation loop: iteratively finds and generalizes patterns until convergence.
 * Robust to empty/invalid parse trees and out-of-bounds access.
 * @param params ADIOS algorithm parameters (eta, alpha, contextSize, overlapThreshold)
 */
void RDSGraph::distill(const ADIOSParams &params)
{
    madios::Logger::trace("Entering RDSGraph::distill");
    if (!quiet) {
        std::cout << "eta = " << params.eta << endl;
        std::cout << "alpha = " << params.alpha << endl;
        std::cout << "contextSize = " << params.contextSize << endl;
        std::cout << "overlapThreshold = " << params.overlapThreshold << endl;
    }
    unsigned int iteration = 0;
    while(true)
    {
        madios::Logger::trace("RDSGraph::distill iteration " + std::to_string(iteration));
        bool foundPattern = false;
        for(const auto& path : paths)
        {
            madios::Logger::trace("RDSGraph::distill: working on Path of length " + std::to_string(path.size()));
            if((params.contextSize < 3) || (path.size() < params.contextSize))
            {
                madios::Logger::trace("RDSGraph::distill: using distill(SearchPath) for path of length " + std::to_string(path.size()));
                bool foundAnotherPattern = distill(path, params);
                foundPattern = foundAnotherPattern || foundPattern;
            }
            else
            {
                madios::Logger::trace("RDSGraph::distill: using generalise(SearchPath) for path of length " + std::to_string(path.size()));
                bool foundAnotherPattern = generalise(path, params);
                foundPattern = foundAnotherPattern || foundPattern;
            }
        }
        if(!foundPattern) {
            madios::Logger::trace("RDSGraph::distill: no new patterns found, breaking loop");
            break;
        }
        iteration++;
    }
    estimateProbabilities();
    // Output node counts for debugging, with robust guards
    if (!quiet) std::cout << endl << endl << endl;
    for(const auto& countVec : counts)
        if(countVec.size() > 1)
        {
            if (!quiet) {
                std::cout << printNodeName(&countVec - &counts[0]);
                std::cout <<  " ---> [";
                for(auto j = 0u; j < countVec.size(); j++) {
                    std::cout << countVec[j];
                    if(j < countVec.size() - 1)
                        std::cout << " | ";
                }
                std::cout << "]";
                std::cout << endl;
            }
        }
    if (!quiet) std::cout << endl << endl << endl;
    if (!quiet) {
        std::cout << endl << endl << endl;
        if (!trees.empty() && trees[0].nodes().size() > 0) {
            trees[0].print(0, 0);
        } else {
            std::cout << "No parse trees to print (empty or invalid)." << std::endl;
        }
        std::cout << endl << endl << endl;
    }
    madios::Logger::trace("Exiting RDSGraph::distill");
}

/**
 * @brief Output the learned PCFG rules in a standard format.
 * Probabilities are normalized over all rules with the same LHS.
 * @param out Output stream to write PCFG rules.
 */
void RDSGraph::convert2PCFG(ostream &out) const
{
    madios::Logger::trace("Entering RDSGraph::convert2PCFG");

    // Output the learned PCFG rules in standard format: LHS -> RHS [probability]
    // Probabilities are normalized over all rules with the same LHS.
    for(const auto& node : nodes)
    {
        if(node.type == LexiconTypes::EC)
        {
            auto ec = static_cast<EquivalenceClass *>(node.lexicon.get()); // use auto
            double total = 0.0;
            for(auto j = 0u; j < ec->size(); j++)
                total += counts[&node - &nodes[0]][j];
            if (total == 0.0) total = 1.0; // avoid division by zero
            for(auto j = 0u; j < ec->size(); j++) {
                double prob = counts[&node - &nodes[0]][j] / total;
                out << "E" << (&node - &nodes[0]) << " -> " << printNodeName((*ec)[j]) << " [" << prob << "]" << std::endl;
            }
        }
        else if(node.type == LexiconTypes::SP)
        {
            auto sp = static_cast<SignificantPattern *>(node.lexicon.get()); // use auto
            double total = counts[&node - &nodes[0]][0];
            if (total == 0.0) total = 1.0;
            double prob = counts[&node - &nodes[0]][0] / total;
            out << "P" << &node - &nodes[0] << " ->";
            for(auto j = 0u; j < sp->size(); j++)
                out << " " << printNodeName((*sp)[j]);
            out << " [" << prob << "]" << std::endl;
        }
    }
    // --- Normalize S rules ---
    // Count occurrences of each unique S rule (RHS)
    std::map<std::vector<std::string>, int> s_rule_counts;
    int total_s_rule_count = 0;
    for(auto i = 0u; i < paths.size(); i++) {
        std::vector<std::string> rhs;
        for(auto j = 1u; j < paths[i].size()-1; j++)
            rhs.push_back(printNodeName(paths[i][j]));
        s_rule_counts[rhs]++;
        total_s_rule_count++;
    }
    // Output S rules with normalized probabilities
    for(const auto& pair : s_rule_counts) {
        const auto& rhs = pair.first;
        int count = pair.second;
        double prob = (total_s_rule_count > 0) ? (static_cast<double>(count) / total_s_rule_count) : 1.0;
        out << "S ->";
        for(const auto& sym : rhs)
            out << " " << sym;
        out << " [" << prob << "]" << std::endl;
    }

    madios::Logger::trace("Exiting RDSGraph::convert2PCFG");
}

// RDSGraph::generate
// Generate a random sequence from the learned grammar.
// Robust to out-of-bounds node indices.
vector<string> RDSGraph::generate(unsigned int node) const
{
    madios::Logger::trace("Entering RDSGraph::generate(unsigned int)");

    if (node >= nodes.size()) {
        madios::Logger::error("[RDSGraph::generate] node index out of bounds (" + std::to_string(node) + "/" + std::to_string(nodes.size()) + ")");
        return {};
    }
    vector<string> sequence;
    if(nodes[node].type == LexiconTypes::Start)
        sequence.push_back("*");
    else if(nodes[node].type == LexiconTypes::End)
        sequence.push_back("#");
    else if(nodes[node].type == LexiconTypes::Symbol)
        sequence.push_back((static_cast<BasicSymbol *>(nodes[node].lexicon.get()))->getSymbol());
    else if(nodes[node].type == LexiconTypes::EC)
    {
        EquivalenceClass *ec = static_cast<EquivalenceClass *>(nodes[node].lexicon.get());
        unsigned int numberOfUnits = ec->size();
        unsigned int randomUnit = static_cast<unsigned int>(floor(numberOfUnits * uniform_rand()));
        vector<string> segment = generate(ec->at(randomUnit));
        sequence.insert(sequence.end(), segment.begin(), segment.end());
    }
    else if(nodes[node].type == LexiconTypes::SP)
    {
         SignificantPattern *SP = static_cast<SignificantPattern *>(nodes[node].lexicon.get());
         for(unsigned int i = 0; i < SP->size(); i++)
         {
             vector<string> segment = generate((*SP)[i]);
             sequence.insert(sequence.end(), segment.begin(), segment.end());
         }
    }
    else
        assert(false);
    assert(sequence.size() > 0);
    madios::Logger::trace("Exiting RDSGraph::generate(unsigned int)");
    return sequence;
}

// Generate a sequence from a specific search path
std::vector<std::string> RDSGraph::generate(const SearchPath &search_path) const
{
    madios::Logger::trace("Entering RDSGraph::generate(SearchPath)");

    std::vector<std::string> sequence;
    for (unsigned int idx : search_path) {
        if (idx >= nodes.size()) {
            madios::Logger::error("[RDSGraph::generate(SearchPath)] node index out of bounds (" + std::to_string(idx) + "/" + std::to_string(nodes.size()) + ")");
            continue;
        }
        if (nodes[idx].type == LexiconTypes::Start)
            sequence.push_back("*");
        else if (nodes[idx].type == LexiconTypes::End)
            sequence.push_back("#");
        else if (nodes[idx].type == LexiconTypes::Symbol)
            sequence.push_back((static_cast<BasicSymbol *>(nodes[idx].lexicon.get()))->getSymbol());
        else if (nodes[idx].type == LexiconTypes::EC)
        {
            EquivalenceClass *ec = static_cast<EquivalenceClass *>(nodes[idx].lexicon.get());
            if (ec->size() > 0) {
                unsigned int randomUnit = static_cast<unsigned int>(floor(ec->size() * uniform_rand()));
                std::vector<std::string> segment = generate(ec->at(randomUnit));
                sequence.insert(sequence.end(), segment.begin(), segment.end());
            }
        }
        else if (nodes[idx].type == LexiconTypes::SP)
        {
            SignificantPattern *sp = static_cast<SignificantPattern *>(nodes[idx].lexicon.get());
            for (unsigned int i = 0; i < sp->size(); i++) {
                std::vector<std::string> segment = generate((*sp)[i]);
                sequence.insert(sequence.end(), segment.begin(), segment.end());
            }
        }
    }
    madios::Logger::trace("Exiting RDSGraph::generate(SearchPath)");
    return sequence;
}

// RDSGraph::distill
// Look for possible significant pattern found with help of equivalence class
bool RDSGraph::distill(const SearchPath &search_path, const ADIOSParams &params)
{
    madios::Logger::trace("RDSGraph::distill(SearchPath) called");
    ConnectionMatrix connections;
    TNT::Array2D<double> flows, descents;
    computeConnectionMatrix(connections, search_path);
    computeDescentsMatrix(flows, descents, connections);
    vector<Range> patterns;
    vector<SignificancePair> pvalues;
    if(!findSignificantPatterns(patterns, pvalues, connections, flows, descents, params.eta, params.alpha)) {
        madios::Logger::trace("RDSGraph::distill(SearchPath): no significant patterns found");
        return false;
    }
    SignificantPattern bestPattern(search_path(patterns.front().first, patterns.front().second));
    madios::Logger::trace("RDSGraph::distill(SearchPath): best pattern found, range = [" + std::to_string(patterns.front().first) + ", " + std::to_string(patterns.front().second) + "]");
    vector<Connection> connectionsToRewire = getRewirableConnections(connections, patterns.front(), params.alpha);
    madios::Logger::trace("RDSGraph::distill(SearchPath): rewiring " + std::to_string(connectionsToRewire.size()) + " connections");
    rewire(connectionsToRewire, SignificantPattern(bestPattern));
    if (!quiet) {
        std::cout << "BEST PATTERN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        std::cout << "RANGE = [" << patterns.front().first << " " << patterns.front().second << "]" << endl;
        std::cout << bestPattern << " with " << "[" << pvalues.front().first << " " << pvalues.front().second << "]" << endl;
        std::cout << connectionsToRewire.size() << " connections rewired." << endl;
        std::cout << "END BEST PATTERN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    }

    return true;
}

// RDSGraph::generalise
// Generalize the given search path by finding and applying equivalence classes.
// Bootstrapping stage followed by generalization and distillation stages.
bool RDSGraph::generalise(const SearchPath &search_path, const ADIOSParams &params)
{
    // BOOTSTRAPPING STAGE
    // bootstrapping variables
    vector<Range> all_boosted_contexts;
    vector<SearchPath> all_boosted_paths;
    vector<vector<EquivalenceClass> > all_encountered_ecs;

    // initialise with just the search path with no bootstrapping
    all_boosted_contexts.push_back(Range(0, 0));
    all_boosted_paths.push_back(search_path);
    all_encountered_ecs.push_back(vector<EquivalenceClass>(max(static_cast<unsigned int>(0), params.contextSize-2)));

    // get all boosted paths
    for(unsigned int i = 0; (i+params.contextSize-1) < search_path.size(); i++)
    {
        Range context(i, i+params.contextSize-1);
        all_encountered_ecs.push_back(vector<EquivalenceClass>());
        SearchPath boosted_part = bootstrap(all_encountered_ecs.back(), SearchPath(search_path(context.first, context.second)), params.overlapThreshold);
        SearchPath boosted_path(search_path.substitute(context.first, context.second, boosted_part));
        all_boosted_contexts.push_back(context);
        all_boosted_paths.push_back(boosted_path);
    }



    // GENERALISATION STAGE
    // generalisation variables
    vector<unsigned int> general2boost;
    vector<unsigned int> all_general_slots;
    vector<SearchPath> all_general_paths;
    vector<EquivalenceClass> all_general_ecs;

    // initialise with just the search path with no generalisation
    general2boost.push_back(0);
    all_general_slots.push_back(0);
    all_general_paths.push_back(search_path);
    all_general_ecs.push_back(EquivalenceClass());

    // get all generalised paths
    for(unsigned int i = 1; i < all_boosted_paths.size(); i++)
    {
        unsigned int context_start = all_boosted_contexts[i].first;
        unsigned int context_finish = all_boosted_contexts[i].second;
        SearchPath boosted_part(all_boosted_paths[i](context_start, context_finish));

        // try all the possible slots
        unsigned int start_index = all_general_paths.size();
        for(unsigned int j = 1; j < params.contextSize-1; j++)
        {
            EquivalenceClass ec = computeEquivalenceClass(boosted_part, j);

            // test that the found equivalence class actually has more than one element
            SearchPath general_path = all_boosted_paths[i];
            if(ec.size() > 1)   // check if found equivalence class is very similar to an existing EC, need to double check
                general_path[context_start+j] = findExistingEquivalenceClass(ec);

            // if general_path is the same as the original search path, no need to test it
            if(general_path == search_path)
                continue;

            // if general_path is already one of the path found for this boosted path, no need to test it
            bool repeated = false;
            for(unsigned int k = start_index; k < all_general_paths.size(); k++)
                if(general_path == all_general_paths[k])
                {
                    repeated = true;
                    break;
                }
            if(repeated) continue;

            // added the generalised path sto the list to be tested
            general2boost.push_back(i);  // add the boosted path number corresponding to the general path
            all_general_slots.push_back(context_start+j);  // stores the slot that was generalised
            all_general_paths.push_back(general_path);
            all_general_ecs.push_back(ec);
        }
    }
    if (!quiet) std::cerr << all_general_paths.size() << " paths tested" << endl;



    // DISTILLATION STAGE
    // significant pattern variables
    vector<Range> all_patterns;
    vector<SignificancePair> all_pvalues;
    vector<unsigned int> pattern2general;

    // Re-enable simulation using a temporary graph clone for new ECs
    for(unsigned int i = 0; i < all_general_paths.size(); i++)
    {
        ConnectionMatrix connections;
        unsigned int slot_index = all_general_slots[i];
        if(all_general_paths[i][slot_index] >= nodes.size()) // if a new EC is expected, simulate with a temp graph
        {
            // Use a temporary graph clone to simulate rewiring for new ECs
            auto temp_graph = this->clone();
            temp_graph->rewire(vector<Connection>(), EquivalenceClass(all_general_ecs[i]));
            temp_graph->computeConnectionMatrix(connections, all_general_paths[i]);
        }
        else
            computeConnectionMatrix(connections, all_general_paths[i]);

        // compute flows and descents matrix from connection matrix
        TNT::Array2D<double> flows, descents;
        computeDescentsMatrix(flows, descents, connections);

        // look for significant patterns
        vector<Range> some_patterns;
        vector<SignificancePair> some_pvalues;
        if(!findSignificantPatterns(some_patterns, some_pvalues, connections, flows, descents, params.eta, params.alpha))
            continue;

        // add them to the list
        for(unsigned int j = 0; j < some_patterns.size(); j++)
        {   // only accept the pattern if any completely new equivalence class is in the distilled pattern
            if(all_general_paths[i][all_general_slots[i]] >= nodes.size())
                if((all_general_slots[i] < some_patterns[j].first) || (all_general_slots[i] > some_patterns[j].second))
                    continue;

            all_patterns.push_back(some_patterns[j]);
            all_pvalues.push_back(some_pvalues[j]);
            pattern2general.push_back(i);
        }
    }



    // LOOK FOR MOST SIGNIFICANT PATTERNS
    bool best_pattern_found = false;
    unsigned int best_pattern_index = all_patterns.size();
    for(unsigned int i = 0; i < all_patterns.size(); i++)
    {
        unsigned int current_pattern_length = all_patterns[i].second - all_patterns[i].first;
        if(best_pattern_found)
        {
            unsigned int best_pattern_length = all_patterns[best_pattern_index].second - all_patterns[best_pattern_index].first;
//             if(current_pattern_length >= best_pattern_length)
//             {
//                 if(current_pattern_length == best_pattern_length)
//                 {
                    if(!(all_pvalues[i] < all_pvalues[best_pattern_index]))
                        continue;
//                 }
//                 else
//                     continue;
//             }
        }

        best_pattern_found = true;
        best_pattern_index = i;
    }
    if(!best_pattern_found)
        return false;
    assert(best_pattern_index < all_patterns.size());
    if (!quiet) std::cout << all_patterns.size() << " patterns found" << endl;

    // get alll the information about the best pattern
    Range best_pattern = all_patterns[best_pattern_index];
    SignificancePair best_pvalues = all_pvalues[best_pattern_index];

    unsigned int best_general_index = pattern2general[best_pattern_index];
    SearchPath best_path = all_general_paths[best_general_index];
    //unsigned int best_slot = all_general_slots[best_general_index];
    EquivalenceClass best_ec = all_general_ecs[best_general_index];

    unsigned int best_boosted_index = general2boost[best_general_index];
    Range best_context = all_boosted_contexts[best_boosted_index];
    vector<EquivalenceClass> best_encountered_ecs = all_encountered_ecs[best_boosted_index];



    // REWIRING STAGE
    if (!quiet) std::cerr << "STARTS REWIRING" << endl;
    unsigned int old_num_nodes = nodes.size();
    unsigned int search_start = max(best_pattern.first, best_context.first);
    unsigned int search_finish = min(best_pattern.second, best_context.second);
    for(unsigned int i = search_start; i <= search_finish; i++)
    {
        if(best_path[i] >= old_num_nodes)       // true if a new EC was discovered at the specific slot
        {
            best_path[i] = nodes.size();
            rewire(vector<Connection>(), EquivalenceClass(best_ec));
        }
        else if(best_path[i] != search_path[i]) // true if the part of the context was boosted from existing ECs
        {
            unsigned int local_slot = i - (best_context.first + 1);
            EquivalenceClass *best_exisiting_ec = static_cast<EquivalenceClass *>(nodes[best_path[i]].lexicon.get());
            EquivalenceClass overlap_ec = best_encountered_ecs[local_slot].computeOverlapEC(*best_exisiting_ec);
            double overlap_ratio = overlap_ec.size() / best_exisiting_ec->size();

            if(overlap_ratio < 1.0)            // true if the overlap with existing EC is less than 1.0, only use the subset that overlaps with it
            {
                if (!quiet) std::cerr << "NEW OVERLAP EC USED: E[" << printEquivalenceClass(overlap_ec) << "]" << endl;
                best_path[i] = nodes.size();
                rewire(vector<Connection>(), EquivalenceClass(overlap_ec));
            }
            else
            {
                if (!quiet) std::cerr << "OLD OVERLAP EC USED: E[" << printNode(best_path[i]) << "]" << endl;
                //rewire(vector<Connection>(), best_path[i]);
            }
        }
    }
    ConnectionMatrix best_connections;
    computeConnectionMatrix(best_connections, best_path);
    vector<Connection> best_pattern_connections = getRewirableConnections(best_connections, best_pattern, params.alpha);
    rewire(best_pattern_connections, SignificantPattern(best_path(best_pattern.first, best_pattern.second)));
    if (!quiet) std::cerr << best_pattern_connections .size() << " occurences rewired" << endl;
    if (!quiet) std::cerr << "ENDS REWIRING" << endl;

    return true;
}

string RDSGraph::toString() const
{
    ostringstream sout;

    sout << "Search Paths" << endl;
    for(unsigned int i = 0; i < paths.size(); i++)
        sout << printPath(paths[i]) << endl;

    sout << endl << "RDS Graph Nodes " << nodes.size() << endl;
    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        sout << "Lexicon " << i << ": " << printNode(i) << "   ------->  " << nodes[i].parents.size() << "  [";
        for(unsigned int j = 0; j < nodes[i].parents.size(); j++)
        {
            sout << nodes[i].parents[j].first;// << "." << nodes[i].parents[j].second;
            if(j < (nodes[i].parents.size() - 1)) sout << "   ";
        }
        sout << "]" << endl;
    }

    return sout.str();
}

// Build the initial graph from input sequences
// Defensive: handles empty sequences and updates internal state consistently
void RDSGraph::buildInitialGraph(const vector<vector<string> > &sequences)
{   //pad the temporary lexicon vector with empty element to acount for special start and end state
    vector<string> lexicon;
    lexicon.push_back("");
    lexicon.push_back("");

    //insert the special symbols
    nodes.push_back(RDSNode(std::make_unique<StartSymbol>(), LexiconTypes::Start));
    nodes.push_back(RDSNode(std::make_unique<EndSymbol>(), LexiconTypes::End));
    for(unsigned int i = 0; i < sequences.size(); i++)
    {
        vector<unsigned int> currentPath;

        //insert start state
        currentPath.push_back(0);

        //create the main part of the graph
        for(unsigned int j = 0; j < sequences[i].size(); j++)
        {
            // Modernized: use std::find instead of manual loop
            auto foundPosition = std::find(lexicon.begin(), lexicon.end(), sequences[i][j]);
            if(foundPosition == lexicon.end())
            {
                lexicon.push_back(sequences[i][j]);
                nodes.push_back(RDSNode(std::make_unique<BasicSymbol>(sequences[i][j]), LexiconTypes::Symbol));
                currentPath.push_back(lexicon.size() - 1);
            }
            else
                currentPath.push_back(std::distance(lexicon.begin(), foundPosition)); //+2 offset for start and end states
        }

        //insert end state
        currentPath.push_back(1);

        paths.push_back(SearchPath(currentPath));
    }

    updateAllConnections();

    // create initial parse trees
    for(unsigned int i = 0; i < paths.size(); i++)
        trees.push_back(ParseTree<unsigned int>(paths[i]));
}

// RDSGraph::computeConnectionMatrix
// Calculate the connection matrix for a given search path.
// Dimensionality: len(search_path) x len(search_path)
// Defensive: handles empty search paths and updates connections matrix in place
void RDSGraph::computeConnectionMatrix(ConnectionMatrix &connections, const SearchPath &search_path) const
{
    // calculate subpath distributions, symmetrical matrix
    unsigned dim = search_path.size();
    connections = ConnectionMatrix(dim, dim);
    for(unsigned int i = 0; i < dim; i++)
    {
        connections(i, i) = getAllNodeConnections(search_path[i]);

        // compute the column from the diagonal
        for(unsigned int j = i + 1; j < dim; j++)
        {
            connections(j, i) = filterConnections(connections(j - 1, i), j-i, SearchPath(search_path(j, j)));
            connections(i, j) = connections(j, i);
        }
    }
}

// RDSGraph::computeEquivalenceClass
// Compute the equivalence class for a given search path and slot index.
// Defensive: checks slot index bounds and ensures valid equivalence class construction
EquivalenceClass RDSGraph::computeEquivalenceClass(const SearchPath &search_path, unsigned int slotIndex) const
{
    if (!(0 < slotIndex && slotIndex < (search_path.size()-1))) {
        throw std::out_of_range("RDSGraph::computeEquivalenceClass: slotIndex out of valid range");
    }

    // get the candidate connections
    vector<Connection> equivalenceConnections = getAllNodeConnections(search_path[0]);
    equivalenceConnections = filterConnections(equivalenceConnections, 0,           SearchPath(search_path(0, slotIndex-1)));
    equivalenceConnections = filterConnections(equivalenceConnections, slotIndex+1, SearchPath(search_path(slotIndex+1, search_path.size()-1)));

    //build equivalence class
    EquivalenceClass ec;
    for(unsigned int i = 0; i < equivalenceConnections.size(); i++)
    {
        unsigned int currentPath = equivalenceConnections[i].first;
        unsigned int currentStart = equivalenceConnections[i].second;

        equivalenceConnections[i].second = currentStart + slotIndex;
        ec.add(paths[currentPath][equivalenceConnections[i].second]);
    }

    return ec;
}

// RDSGraph::bootstrap
// Bootstrap the search path by finding initial equivalence classes based on overlaps.
// Defensive: handles empty or too short search paths, updates encountered_ecs in place
SearchPath RDSGraph::bootstrap(vector<EquivalenceClass> &encountered_ecs, const SearchPath &search_path, double overlapThreshold) const
{
    // find all possible connections
    vector<Connection> equivalenceConnections = filterConnections(getAllNodeConnections(search_path[0]), search_path.size()-1, SearchPath(search_path(search_path.size()-1, search_path.size()-1)));

    // find potential ECs
    encountered_ecs.clear();
    for(unsigned int i = 1; i < search_path.size()-1; i++)
    {
        encountered_ecs.push_back(EquivalenceClass());
        for(unsigned int j = 0; j < equivalenceConnections.size(); j++)
        {
            unsigned int currentPath = equivalenceConnections[j].first;
            unsigned int currentStart = equivalenceConnections[j].second;

            encountered_ecs.back().add(paths[currentPath][currentStart+i]);
        }
    }

    // init bootstrap data
    vector<unsigned int> overlap_ecs = search_path(1, search_path.size()-2);
    vector<double> overlap_ratios(search_path.size()-2, 0.0);

    // bootstrap search path
    SearchPath bootstrap_path = search_path;
    for(unsigned int i = 0; i < encountered_ecs.size(); i++)
    {
        for(unsigned int j = 0; j < nodes.size(); j++)
            if(nodes[j].type == LexiconTypes::EC)
            {
                EquivalenceClass *ec = static_cast<EquivalenceClass *>(nodes[j].lexicon.get());
                double overlap = encountered_ecs[i].computeOverlapEC(*ec).size()/static_cast<double>(ec->size());
                if((overlap > overlap_ratios[i]) && (overlap > overlapThreshold))
                {
                    overlap_ecs[i] = j;
                    overlap_ratios[i] = overlap;
                }
            }
        bootstrap_path[i + 1] = overlap_ecs[i];
    }

    return bootstrap_path;
}

// RDSGraph::computeDescentsMatrix
// Compute the descents matrix (D_R and D_L) for the connection matrix.
// Dimensionality: len(connections) x len(connections)
// Defensive: handles empty connections matrices and updates descents in place
void RDSGraph::computeDescentsMatrix(TNT::Array2D<double> &flows, TNT::Array2D<double> &descents, const ConnectionMatrix &connections) const
{
    // calculate P_R and P_L
    unsigned dim = connections.dim1();
    flows = TNT::Array2D<double>(dim, dim, -1.0);
    for(unsigned int i = 0; i < dim; i++)
        for(unsigned int j = 0; j < dim; j++)
            if(i > j)
                flows(i, j) = static_cast<double>(connections(i, j).size()) / connections(i-1, j).size();
            else if(i < j)
                flows(i, j) = static_cast<double>(connections(i, j).size()) / connections(i+1, j).size();
            else
                flows(i, j) = static_cast<double>(connections(i, j).size()) / corpusSize;

    // calculate D_R and D_L
    descents = TNT::Array2D<double>(dim, dim, -1.0);
    for(unsigned int i = 0; i < dim; i++)
        for(unsigned int j = 0; j < dim; j++)
            if(i > j)
                descents(i, j) = flows(i, j) / flows(i-1, j);
            else if(i < j)
                descents(i, j) = flows(i, j) / flows(i+1, j);
            else
                descents(i, j) = 1.0;
}

// RDSGraph::findSignificantPatterns
// Find significant patterns in the given connection, flows, and descents matrices.
// Updates patterns and pvalues with the found patterns and their significance.
// Returns true if any patterns were found, false otherwise.
bool RDSGraph::findSignificantPatterns(std::vector<Range> &patterns, std::vector<SignificancePair> &pvalues, const ConnectionMatrix &connections, const TNT::Array2D<double> &flows, const TNT::Array2D<double> &descents, double eta, double alpha) const
{
    patterns.clear();
    pvalues.clear();

    //find candidate pattern start and ends
    unsigned int pathLength = descents.dim1();
    vector<unsigned int> candidateEndRows;
    vector<unsigned int> candidateStartRows;
    for(int i = 0; i < descents.dim1(); i++)
    {
        for(int j = i - 1; j >= 0; j--)
            if(descents(i, j) < eta)
            {
                candidateEndRows.push_back(i - 1);
                break;
            }

        for(int j = i + 1; j < descents.dim2(); j++)
            if(descents(i, j) < eta)
            {
                candidateStartRows.push_back(i + 1);
                break;
            }
    }

    //find candidate patterns;
    vector<Range> candidatePatterns;
    for(unsigned int i = 0; i < candidateStartRows.size(); i++)
        for(unsigned int j = 0; j < candidateEndRows.size(); j++)
            if(candidateStartRows[i] < candidateEndRows[j])
                candidatePatterns.push_back(Range(candidateStartRows[i], candidateEndRows[j]));

    //for(unsigned int i = 0; i < candidatePatterns.size(); i++)
    //    std::cout << "Candidate Pattern " << i << " = " << candidatePatterns[i].first << " " << candidatePatterns[i].second << endl;

    TNT::Array2D<double> pvalueCache(pathLength, pathLength, 2.0);
    for(unsigned int i = 0; i < candidatePatterns.size(); i++)
    {   //std::cout << "START+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        //std::cout << "Testing pattern at [" << candidatePatterns[i].first << " -> " << candidatePatterns[i].second << "]" << endl;

        SignificancePair tempPvalues(2.0, 2.0);
        pair<unsigned int, unsigned int> descentContexts;
        tempPvalues.second = findBestRightDescentColumn(descentContexts.second, pvalueCache, connections, flows, descents, candidatePatterns[i], eta);
        tempPvalues.first = findBestLeftDescentColumn(descentContexts.first, pvalueCache, connections, flows, descents, candidatePatterns[i], eta);
        if((fabs(tempPvalues.first) > 1.0) || (fabs(tempPvalues.second) > 1.0)) continue;

        //std::cout << "P_R at " << candidatePatterns[i].second << " " << descentContexts.second << " = " << flows(candidatePatterns[i].second, descentContexts.second) << endl;
        //std::cout << "right pvalue = " << 1.0 - tempPvalues.second << " for " << connections(candidatePatterns[i].second + 1, descentContexts.second).size() << " out of " << connections(candidatePatterns[i].second, descentContexts.second).size() << endl;

        //std::cout << "P_L at " << candidatePatterns[i].first << " " << descentContexts.first << " = " << flows(candidatePatterns[i].first, descentContexts.first) << endl;
        //std::cout << "left pvalue = " << 1.0 - tempPvalues.first << " for " << connections(candidatePatterns[i].first - 1, descentContexts.first).size() << " out of " << connections(candidatePatterns[i].first, descentContexts.first).size() << endl;

        // pattern IS significant
        if(isPatternSignificant(tempPvalues, alpha))
        {
            //std::cout << "Pattern is significant at [" << 1-tempPvalues.first << " --- " << 1-tempPvalues.second << "]" << endl;
            //std::cout << "Right descent context is [" << descentContexts.second << " -> " << candidatePatterns[i].second << "]" << endl;
            //std::cout << "Left descent context is [" << descentContexts.first << " -> " << candidatePatterns[i].first << "]" << endl;

            patterns.push_back(candidatePatterns[i]);
            pvalues.push_back(tempPvalues);

            //found a MORE significant pattern
            if((patterns.size() == 1) || (pvalues.back() < pvalues.front()))
            {
                swap(patterns.front(), patterns.back());
                swap(pvalues.front(), pvalues.back());
                //std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!found a better SP" << endl;
            }
        }
        //std::cout << "END----------------------------------------------------------------------------------------------" << endl;
    }

    return patterns.size() > 0;
}

// RDSGraph::getRewirableConnections
// Extract the connections that can be rewired for a given significant pattern.
// Defensive: ensures valid pattern range and alpha threshold
vector<Connection> RDSGraph::getRewirableConnections(const ConnectionMatrix &connections, const Range &bestSP, double alpha) const
{
    vector<Connection> validConnections = connections(bestSP.second, bestSP.first);

    return validConnections;
}

// RDSGraph::rewire
// Rewire the graph for a given equivalence class and connection set.
// Overloaded for different rewire targets (EC, SP, or node index).
void RDSGraph::rewire(const std::vector<Connection> &connections, unsigned int ec)
{
    if (ec >= nodes.size() || nodes[ec].type != LexiconTypes::EC) {
        throw std::invalid_argument("RDSGraph::rewire: ec index invalid or not an EC node");
    }

    for(unsigned int i = 0; i < connections.size(); i++)
        paths[connections[i].first][connections[i].second] = ec;

    updateAllConnections();
}

void RDSGraph::rewire(const vector<Connection> &connections, const EquivalenceClass &ec)
{
    nodes.push_back(RDSNode(std::make_unique<EquivalenceClass>(ec), LexiconTypes::EC));
    rewire(connections, nodes.size() - 1);
}

void RDSGraph::rewire(const vector<Connection> &connections, const SignificantPattern &sp)
{
    nodes.push_back(RDSNode(std::make_unique<SignificantPattern>(sp), LexiconTypes::SP));
    const SignificantPattern &pattern = sp;

    if (connections.empty()) {
        std::cerr << "[RDSGraph::rewire] Warning: empty connections vector." << std::endl;
        return;
    }
    unsigned int pattern_size = pattern.size();

    // remove any overlapping connections
    vector<Connection> sorted_connections;
    for(unsigned int i = 0; i < connections.size(); i++)
    {
        unsigned int current_path_index = connections[i].first;
        unsigned int current_path_pos = connections[i].second;

        bool found_group = false, inserted = false;
        for(unsigned int j = 0; j < sorted_connections.size(); j++)
            if(current_path_index == sorted_connections[j].first)
            {
                found_group = true;
                if(current_path_pos < sorted_connections[j].second)
                {
                    sorted_connections.insert(sorted_connections.begin()+j, connections[i]);
                    inserted = true;
                    break;
                }
            }
            else if(found_group)
            {
                sorted_connections.insert(sorted_connections.begin()+j, connections[i]);
                inserted = true;
                break;
            }

        if(!inserted)
            sorted_connections.push_back(connections[i]);
    }

    // validate the sorted connections
    if (sorted_connections.empty()) {
        std::cerr << "[RDSGraph::rewire] Warning: sorted_connections is empty." << std::endl;
        return;
    }
    vector<Connection> valid_connections;
    valid_connections.push_back(sorted_connections.front());
    for(unsigned int i = 1; i < sorted_connections.size(); i++)
    {
        unsigned int current_path_index = sorted_connections[i].first;
        unsigned int current_path_pos = sorted_connections[i].second;
        unsigned int last_path_index = valid_connections.back().first;
        unsigned int last_path_pos = valid_connections.back().second;

        // the path is the same as the last path the pattern overlaps with the last pattern then do not rewire it
        if((current_path_index == last_path_index) && (current_path_pos <= (last_path_pos+pattern_size-1)))
            continue;

        valid_connections.push_back(sorted_connections[i]);
    }
    if (!quiet) std::cout << valid_connections.size() << " valid_connections" << endl;

    // rewire the connections in reverse order to avoid problems with path changing size
    for(unsigned int i = valid_connections.size()-1; i < valid_connections.size(); i--)
    {
        unsigned int path_index = valid_connections[i].first;
        unsigned int path_pos = valid_connections[i].second;

        if (path_index >= paths.size()) {
            std::cerr << "[RDSGraph::rewire] Warning: path_index out of bounds (" << path_index << "/" << paths.size() << ")" << std::endl;
            continue;
        }
        if (path_pos + pattern_size - 1 >= paths[path_index].size()) {
            std::cerr << "[RDSGraph::rewire] Warning: path_pos out of bounds (" << path_pos << "/" << paths[path_index].size() << ")" << std::endl;
            continue;
        }
        // rewiring the parse trees
        SearchPath segment(paths[path_index](path_pos, path_pos+pattern_size-1));
        for(unsigned int j = 0; j < segment.size(); j++)
            if(segment[j] != pattern[j])
                trees[path_index].rewire(path_pos+j, path_pos+j, pattern[j]);
        trees[path_index].rewire(path_pos, path_pos+pattern_size-1, nodes.size()-1);

        // rewiring the paths
        paths[path_index].rewire(path_pos, path_pos+pattern_size-1, nodes.size()-1);
    }

    updateAllConnections();
}

// RDSGraph::updateAllConnections
// Update all connections and parent links in the graph after any modification.
// Defensive: ensures consistent internal state
void RDSGraph::updateAllConnections()
{
    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        nodes[i].setConnections(vector<Connection>());
        nodes[i].parents.clear();
    }

    corpusSize = 0;
    for(unsigned int i = 0; i < paths.size(); i++)
    {
        corpusSize += paths[i].size();
         for(unsigned int j = 0; j < paths[i].size(); j++)
             nodes[paths[i][j]].addConnection(Connection(i, j));
    }

    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        if(nodes[i].type == LexiconTypes::SP)
        {
            SignificantPattern *sp = static_cast<SignificantPattern *>(nodes[i].lexicon.get());
            for(unsigned int j = 0; j < sp->size(); j++)
                nodes[sp->at(j)].parents.push_back(Connection(i, sp->find(sp->at(j))));
        }
        else if(nodes[i].type == LexiconTypes::EC)
        {
            EquivalenceClass *ec = static_cast<EquivalenceClass *>(nodes[i].lexicon.get());
            for(unsigned int j = 0; j < ec->size(); j++)
                nodes[ec->at(j)].parents.push_back(Connection(i, 0));
        }
    }
}

// RDSGraph::computeRightSignificance
// Compute the right significance for a given descent point using the connections and flows matrices.
// Defensive: ensures valid row/column indices
double RDSGraph::computeRightSignificance(const ConnectionMatrix &connections, const TNT::Array2D<double> &flows, const pair<unsigned int, unsigned int> &descentPoint, double eta) const
{
    unsigned int row = descentPoint.first;
    unsigned int col = descentPoint.second;
    assert(row > col);

    double significance = 0.0;
    unsigned int patternOccurences = connections(row - 1, col).size();
    unsigned int descentOccurences = connections(row, col).size();
    for(unsigned int i = 0; i <= descentOccurences; i++)
        significance += binom(i, patternOccurences, eta * flows(row - 1, col));

    return min(max(significance, 0.0), 1.0);
}

// RDSGraph::computeLeftSignificance
// Compute the left significance for a given descent point using the connections and flows matrices.
// Defensive: ensures valid row/column indices
double RDSGraph::computeLeftSignificance(const ConnectionMatrix &connections, const TNT::Array2D<double> &flows, const pair<unsigned int, unsigned int> &descentPoint, double eta) const
{
    unsigned int row = descentPoint.first;
    unsigned int col = descentPoint.second;
    assert(row < col);

    double significance = 0.0;
    unsigned int patternOccurences = connections(row + 1, col).size();
    unsigned int descentOccurences = connections(row, col).size();
    for(unsigned int i = 0; i <= descentOccurences; i++)
        significance += binom(i, patternOccurences, eta * flows(row + 1, col));

    return min(max(significance, 0.0), 1.0);;
}

// RDSGraph::findBestRightDescentColumn
// Find the best right descent column for a given pattern and descent context.
// Updates bestColumn with the column index of the best descent.
// Defensive: ensures valid pattern and descent context
double RDSGraph::findBestRightDescentColumn(unsigned int &bestColumn, TNT::Array2D<double> &pvalueCache, const ConnectionMatrix &connections, const TNT::Array2D<double> &flows, const TNT::Array2D<double> &descents, const Range &pattern, double eta) const
{
    double pvalue = 2.0;
    pair<unsigned int, unsigned int> descentPoint(pattern.second + 1, bestColumn);
    for(unsigned int i = 0; i <= pattern.first; i++)
    {
        descentPoint.second = i;
        if(!(descents(descentPoint.first, descentPoint.second) < eta)) continue;
        if(pvalueCache(pattern.second + 1, i) > 1.0)
            pvalueCache(pattern.second + 1, i) = computeRightSignificance(connections, flows, descentPoint, eta);

        if(pvalueCache(pattern.second + 1, i) < pvalue)
        {
            bestColumn = i;
            pvalue = pvalueCache(pattern.second + 1, i);
        }
    }

    return pvalue;
}

// RDSGraph::findBestLeftDescentColumn
// Find the best left descent column for a given pattern and descent context.
// Updates bestColumn with the column index of the best descent.
// Defensive: ensures valid pattern and descent context
double RDSGraph::findBestLeftDescentColumn(unsigned int &bestColumn, TNT::Array2D<double> &pvalueCache, const ConnectionMatrix &connections, const TNT::Array2D<double> &flows, const TNT::Array2D<double> &descents, const Range &pattern, double eta) const
{
    double pvalue = 2.0;
    pair<unsigned int, unsigned int> descentPoint(pattern.first - 1, bestColumn);
    for(int i = pattern.second; i < connections.dim2(); i++)
    {
        descentPoint.second = i;
        if(!(descents(descentPoint.first, descentPoint.second) < eta)) continue;
        if(pvalueCache(pattern.first - 1, i) > 1.0)
            pvalueCache(pattern.first - 1, i) = computeLeftSignificance(connections, flows, descentPoint, eta);

        if(pvalueCache(pattern.first - 1, i) < pvalue)
        {
            bestColumn = i;
            pvalue = pvalueCache(pattern.first - 1, i);
        }
    }

    return pvalue;
}

// RDSGraph::filterConnections
// Filter the initial connections based on the given search path and offset.
// Returns a vector of connections that match the search path segment.
vector<Connection> RDSGraph::filterConnections(const vector<Connection> &init_cons, unsigned int start_offset, const SearchPath &search_path) const
{
    vector<Connection> filtered_cons;
    for(unsigned int i = 0; i < init_cons.size(); i++)
    {
        unsigned int cur_path = init_cons[i].first;
        unsigned int cur_pos = init_cons[i].second;

        // discard current connection because the path is not long enough to match the search path (segment)
        if((cur_pos+start_offset+search_path.size()) > paths[cur_path].size())
            continue;

        unsigned int count = search_path.size();
        for(unsigned int j = 0; j < search_path.size(); j++)
        {
            unsigned int actual_pos = j+cur_pos+start_offset;
            if(nodes[search_path[j]].type == LexiconTypes::EC)
            {   // if node on search path is EC and it contains the node and temp path
                if(!(static_cast<EquivalenceClass *>(nodes[search_path[j]].lexicon.get())->has(paths[cur_path][actual_pos])))
                    break;
            }
            else// else just test if they are the same node (BasicSymbol)
                if(search_path[j] != paths[cur_path][actual_pos])
                    break;

            count--;
        }

        // 0 if search_path completely matches temp_path
        if(count == 0)
            filtered_cons.push_back(init_cons[i]);
    }

    return filtered_cons;
}

// RDSGraph::getAllNodeConnections
// Get all connections for a given node, including those from its equivalence class if applicable.
// Defensive: ensures valid node index and handles empty connection sets
vector<Connection> RDSGraph::getAllNodeConnections(unsigned int nodeIndex) const
{
    if (nodeIndex >= nodes.size()) {
        throw std::out_of_range("RDSGraph::getAllNodeConnections: nodeIndex out of bounds");
    }
    vector<Connection> connections = nodes[nodeIndex].getConnections();

    //get all connections belonging to the nodes in the equivalence class
    if(nodes[nodeIndex].type == LexiconTypes::EC)
    {
        EquivalenceClass *ec = static_cast<EquivalenceClass *>(nodes[nodeIndex].lexicon.get());
        for(unsigned int i = 0; i < ec->size(); i++)
        {
            vector<Connection> tempConnections = nodes[ec->at(i)].getConnections();
            connections.insert(connections.end(), tempConnections.begin(), tempConnections.end());
        }
    }

    return connections;
}

// RDSGraph::findExistingEquivalenceClass
// Find an existing equivalence class that is a subset of the given equivalence class.
// Returns the index of the found equivalence class, or nodes.size() if none found.
unsigned int RDSGraph::findExistingEquivalenceClass(const EquivalenceClass &ec) const
{
    // look for the existing ec that is a subset of the given ec
    for(unsigned int i = 0; i < nodes.size(); i++)
        if(nodes[i].type == LexiconTypes::EC)
        {
            EquivalenceClass *temp_ec = static_cast<EquivalenceClass *>(nodes[i].lexicon.get());
            if(ec.computeOverlapEC(*temp_ec).size() == temp_ec->size())
                return i;
        }

    return nodes.size();
}

// RDSGraph::estimateProbabilities
// Recompute the counts for all nodes based on the current parse trees.
// Defensive: clears counts before filling, robust to out-of-bounds.
void RDSGraph::estimateProbabilities()
{
    counts.clear();
    for(unsigned int i = 0; i < nodes.size(); i++)
        if(nodes[i].type == LexiconTypes::EC)
        {
            EquivalenceClass *ec = static_cast<EquivalenceClass *>(nodes[i].lexicon.get());
            counts.push_back(vector<unsigned int>(ec->size(), 0));
        }
        else
            counts.push_back(vector<unsigned int>(1, 0));
    for(unsigned int i = 0; i < trees.size(); i++)
    {
        const vector<ParseNode<unsigned int> > &tree_nodes = trees[i].nodes();
        for(unsigned int j = 1; j < tree_nodes.size(); j++)
        {
            unsigned int node_index = tree_nodes[j].value();
            if(node_index >= nodes.size()) {
                std::cerr << "[RDSGraph::estimateProbabilities] Warning: node_index out of bounds (" << node_index << "/" << nodes.size() << ")" << std::endl;
                continue;
            }
            if(nodes[node_index].type == LexiconTypes::EC)
            {
                assert(tree_nodes[j].children().size() == 1);
                EquivalenceClass *ec = static_cast<EquivalenceClass *>(nodes[node_index].lexicon.get());
                unsigned int first_child_pos = tree_nodes[j].children().front();
                unsigned int first_child_val = tree_nodes[first_child_pos].value();
                for(unsigned int k = 0; k < ec->size(); k++)
                    if(ec->at(k) == first_child_val && node_index < counts.size() && k < counts[node_index].size())
                        counts[node_index][k]++;
            }
            else if(node_index < counts.size() && 0 < counts[node_index].size())
                counts[node_index][0]++;
        }
    }
}

// RDSGraph::printSignificantPattern
// Print the significant pattern in a human-readable format.
string RDSGraph::printSignificantPattern(const SignificantPattern &sp) const
{
    ostringstream sout;
    for(unsigned int i = 0; i < sp.size(); i++)
    {
        unsigned tempIndex = sp[i];
        if (tempIndex >= nodes.size()) {
            sout << "[INVALID_INDEX:" << tempIndex << "]";
        } else if(nodes[tempIndex].type == LexiconTypes::EC) {
            sout << "E" << tempIndex;
        } else if(nodes[tempIndex].type == LexiconTypes::SP) {
            sout << "P" << tempIndex;
        } else if(nodes[tempIndex].type == LexiconTypes::Symbol) {
            BasicSymbol* sym = static_cast<BasicSymbol*>(nodes[tempIndex].lexicon.get());
            sout << sym->getSymbol();
        } else if(nodes[tempIndex].type == LexiconTypes::Start) {
            sout << "*";
        } else if(nodes[tempIndex].type == LexiconTypes::End) {
            sout << "#";
        } else {
            sout << "[UNKNOWN_TYPE:" << tempIndex << "]";
        }
        // No dashes or spaces between elements
    }
    return sout.str();
}

// RDSGraph::printEquivalenceClass
// Print the equivalence class in a human-readable format.
string RDSGraph::printEquivalenceClass(const EquivalenceClass &ec) const
{
    ostringstream sout;
    for(unsigned int i = 0; i < ec.size(); i++)
    {
        unsigned tempIndex = ec[i];
        if (tempIndex >= nodes.size()) {
            sout << "[INVALID_INDEX:" << tempIndex << "]";
        } else if(nodes[tempIndex].type == LexiconTypes::EC) {
            sout << "E" << tempIndex;
        } else if(nodes[tempIndex].type == LexiconTypes::SP) {
            sout << "P" << tempIndex;
        } else if(nodes[tempIndex].type == LexiconTypes::Symbol) {
            BasicSymbol* sym = static_cast<BasicSymbol*>(nodes[tempIndex].lexicon.get());
            sout << sym->getSymbol();
        } else if(nodes[tempIndex].type == LexiconTypes::Start) {
            sout << "*";
        } else if(nodes[tempIndex].type == LexiconTypes::End) {
            sout << "#";
        } else {
            sout << "[UNKNOWN_TYPE:" << tempIndex << "]";
        }
        if(i < (ec.size() - 1)) sout << ","; // No space after comma
    }
    return sout.str();
}

// RDSGraph::printNode
// Print the node information in a human-readable format.
string RDSGraph::printNode(unsigned int node) const
{
    ostringstream sout;
    if (node >= nodes.size()) {
        sout << "[INVALID_NODE:" << node << "]";
        return sout.str();
    }
    if(nodes[node].type == LexiconTypes::EC)
    {
        EquivalenceClass *ec = static_cast<EquivalenceClass *>(nodes[node].lexicon.get());
        sout << "E[" << printEquivalenceClass(*ec) << "]";
    }
    else if(nodes[node].type == LexiconTypes::SP)
    {
        SignificantPattern *sp = static_cast<SignificantPattern *>(nodes[node].lexicon.get());
        sout << "P[" << printSignificantPattern(*sp) << "]";
    }
    else if(nodes[node].type == LexiconTypes::Symbol)
    {
        BasicSymbol* sym = static_cast<BasicSymbol*>(nodes[node].lexicon.get());
        sout << sym->getSymbol();
    }
    else if(nodes[node].type == LexiconTypes::Start)
    {
        sout << "*";
    }
    else if(nodes[node].type == LexiconTypes::End)
    {
        sout << "#";
    }
    else
    {
        sout << "[UNKNOWN_TYPE:" << node << "]";
    }
    return sout.str();
}

// RDSGraph::printPath
// Print the search path in a human-readable format.
string RDSGraph::printPath(const SearchPath &path) const
{
    ostringstream sout;
    sout << "[";
    for(unsigned int i = 0; i < path.size(); i++)
    {
        unsigned tempIndex = path[i];
        if (tempIndex >= nodes.size()) {
            sout << "[INVALID_INDEX:" << tempIndex << "]";
        } else if(nodes[tempIndex].type == LexiconTypes::EC) {
            sout << "E" << tempIndex;
        } else if(nodes[tempIndex].type == LexiconTypes::SP) {
            sout << "P" << tempIndex;
        } else if(nodes[tempIndex].type == LexiconTypes::Symbol) {
            BasicSymbol* sym = static_cast<BasicSymbol*>(nodes[tempIndex].lexicon.get());
            sout << sym->getSymbol();
        } else if(nodes[tempIndex].type == LexiconTypes::Start) {
            sout << "*";
        } else if(nodes[tempIndex].type == LexiconTypes::End) {
            sout << "#";
        } else {
            sout << "[UNKNOWN_TYPE:" << tempIndex << "]";
        }
        if(i < (path.size() - 1)) sout << " ";
    }
    sout << "]";
    return sout.str();
}

// RDSGraph::printNodeName
// Print the node name (for debugging/output) in a human-readable format.
std::string RDSGraph::printNodeName(unsigned int node) const
{
    ostringstream sout;
    if (node >= nodes.size()) {
        sout << "[INVALID_NODE:" << node << "]";
        return sout.str();
    }
    if(nodes[node].type == LexiconTypes::EC) {
        sout << "E" << node;
    } else if(nodes[node].type == LexiconTypes::SP) {
        sout << "P" << node;
    } else if(nodes[node].type == LexiconTypes::Symbol) {
        BasicSymbol* sym = static_cast<BasicSymbol*>(nodes[node].lexicon.get());
        sout << sym->getSymbol();
    } else if(nodes[node].type == LexiconTypes::Start) {
        sout << "*";
    } else if(nodes[node].type == LexiconTypes::End) {
        sout << "#";
    } else {
        sout << "[UNKNOWN_TYPE:" << node << "]";
    }
    return sout.str();
}

// Utility: Print connections, flows, and descents matrices for debugging
void printInfo(const ConnectionMatrix &connections, const TNT::Array2D<double> &flows, const TNT::Array2D<double> &descents)
{
    std::cout << "Connections:" << std::endl;
    for(int i = 0; i < connections.dim1(); i++)
    {
        for(int j = 0; j < connections.dim2(); j++)
        {
            std::cout << connections(i, j).size() << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
    std::cout << "Flows:" << std::endl;
    for(int i = 0; i < flows.dim1(); i++)
    {
        for(int j = 0; j < flows.dim2(); j++)
        {
            std::cout << flows(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
    std::cout << "Descents:" << std::endl;
    for(int i = 0; i < descents.dim1(); i++)
    {
        for(int j = 0; j < descents.dim2(); j++)
        {
            std::cout << descents(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

std::vector<std::string> RDSGraph::generate() const {
    // By convention, the start node is at index 0
    return generate(0);
}
/**
 * @brief Get the number of significant patterns currently in the graph.
 * @return The number of significant patterns.
 */
unsigned int RDSGraph::getPatternCount() const {
    // Return the number of significant patterns currently in the graph
    return significant_patterns.size();
}
/**
 * @brief Get the number of rewiring operations performed.
 * @return The number of rewiring operations.
 */
unsigned int RDSGraph::getRewiringCount() const {
    // Return the number of rewiring operations performed (tracked by rewiring_ops)
    return rewiring_ops;
}

std::unique_ptr<RDSGraph> RDSGraph::clone() const {
    auto new_graph = std::make_unique<RDSGraph>();
    new_graph->corpusSize = corpusSize;
    new_graph->quiet = quiet;
    new_graph->counts = counts;
    new_graph->significant_patterns = significant_patterns;
    new_graph->rewiring_ops = rewiring_ops;

    // Deep copy nodes
    new_graph->nodes.reserve(nodes.size());
    for (const auto& node : nodes) {
        new_graph->nodes.emplace_back(node); // RDSNode copy constructor does deep copy
    }

    // Copy paths (vector<SearchPath> is copyable)
    new_graph->paths = paths;

    // Copy parse trees (assume ParseTree<unsigned int> is copyable)
    new_graph->trees = trees;

    return new_graph;
}
