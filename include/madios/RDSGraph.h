#pragma once
// File: RDSGraph.h
// Purpose: Declares the RDSGraph class, which implements the main graph structure for pattern discovery in the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#ifndef RDSGRAPH_H
#define RDSGRAPH_H

#include "RDSNode.h"
#include "ADIOSUtils.h"
#include "maths/special.h"
#include "MiscUtils.h"
#include "ParseTree.h"
#include "madios/maths/tnt/array2d.h"

#include <string>
#include <sstream>

/**
 * @brief Check if both p-values in a SignificancePair are less than alpha.
 * @param pvalues The pair of significance values.
 * @param alpha The significance threshold.
 * @return True if both p-values are less than alpha, false otherwise.
 */
bool isPatternSignificant(const SignificancePair &pvalues, double alpha);

/**
 * @brief Less-than operator for SignificancePair.
 * @param a First significance pair.
 * @param b Second significance pair.
 * @return True if a < b, false otherwise.
 */
bool operator<(const SignificancePair &a, const SignificancePair &b);

/**
 * @class RDSGraph
 * @brief Implements the main graph structure for pattern discovery in the ADIOS algorithm.
 *
 * RDSGraph manages nodes, search paths, and significant patterns for grammar induction.
 * It provides methods for distillation, pattern generalization, PCFG conversion, and sequence generation.
 * Copying is disabled due to unique_ptr members; use clone() for deep copies.
 */
class RDSGraph: public Stringable
{
    public:
        /**
         * @brief Default constructor. Creates an empty RDSGraph.
         */
        RDSGraph();
        /**
         * @brief Constructs an RDSGraph from a set of input sequences.
         * @param sequences A vector of input sequences (each sequence is a vector of strings).
         */
        explicit RDSGraph(const std::vector<std::vector<std::string> > &sequences);

        // Copying is not allowed for RDSGraph due to unique_ptr members
        RDSGraph(const RDSGraph& other) = delete;
        RDSGraph& operator=(const RDSGraph& other) = delete;

        /**
         * @brief Generate a random sequence from the learned grammar, starting from the default start node.
         * @return A vector of strings representing the generated sequence.
         */
        std::vector<std::string> generate() const;
        /**
         * @brief Generate a sequence from a specific search path.
         * @param search_path The search path to generate from.
         * @return A vector of strings representing the generated sequence.
         */
        std::vector<std::string> generate(const SearchPath &search_path) const;
        /**
         * @brief Generate a random sequence from the learned grammar, starting from a given node.
         * @param node The index of the node to start from.
         * @return A vector of strings representing the generated sequence.
         */
        std::vector<std::string> generate(unsigned int node) const;
        /**
         * @brief Main distillation loop: iteratively finds and generalizes patterns until convergence.
         * @param params ADIOS algorithm parameters (eta, alpha, contextSize, overlapThreshold)
         */
        void distill(const ADIOSParams &params);
        /**
         * @brief Output the learned PCFG rules in a standard format.
         * Probabilities are normalized over all rules with the same LHS.
         * @param out Output stream to write PCFG rules.
         */
        void convert2PCFG(std::ostream &out) const;
        /**
         * @brief Returns a string representation of the RDSGraph (for debugging).
         * @return A string describing the graph structure.
         */
        virtual std::string toString() const;
        /**
         * @brief Get the search paths in the graph.
         * @return A const reference to the vector of search paths.
         */
        const std::vector<SearchPath>& getPaths() const { return paths; }
        /**
         * @brief Get the nodes in the graph.
         * @return A const reference to the vector of nodes.
         */
        const std::vector<RDSNode>& getNodes() const { return nodes; }
        /**
         * @brief Get a string representation of a node.
         * @param node The node index.
         * @return A string describing the node.
         */
        std::string getNodeString(unsigned int node) const { return printNode(node); }
        /**
         * @brief Get the name of a node (for debugging/output).
         * @param node The node index.
         * @return The node name as a string.
         */
        std::string getNodeName(unsigned int node) const { return printNodeName(node); }
        /**
         * @brief Set the quiet flag (suppress verbose output if true).
         * @param q True to suppress output, false for verbose.
         */
        void setQuiet(bool q) { quiet = q; }
        /**
         * @brief Check if the graph is in quiet mode.
         * @return True if quiet, false otherwise.
         */
        bool isQuiet() const { return quiet; }
        /**
         * @brief Get the number of significant patterns currently in the graph.
         * @return The number of significant patterns.
         */
        unsigned int getPatternCount() const;
        /**
         * @brief Get the number of rewiring operations performed.
         * @return The number of rewiring operations.
         */
        unsigned int getRewiringCount() const;
        /**
         * @brief Create a deep copy of this RDSGraph (for safe simulation/experimentation).
         * @return A unique_ptr to a new RDSGraph that is a deep copy of this one.
         */
        std::unique_ptr<RDSGraph> clone() const;

#ifdef MADIOS_TESTING
    public:
        /**
         * @brief Test-only wrapper for generalise, for unit testing.
         * @param search_path The search path to generalise.
         * @param params The ADIOS parameters.
         * @return True if generalisation succeeded, false otherwise.
         */
        bool testGeneralise(const SearchPath &search_path, const ADIOSParams &params) {
            return generalise(search_path, params);
        }
#endif

    private:
        /**
         * @brief The number of input sequences in the corpus.
         */
        unsigned int corpusSize;
        /**
         * @brief The nodes in the graph.
         */
        std::vector<RDSNode> nodes;
        /**
         * @brief The search paths in the graph.
         */
        std::vector<SearchPath> paths;
        /**
         * @brief Parse trees for each input sequence.
         */
        std::vector<ParseTree<unsigned int> > trees;
        /**
         * @brief Occurrence counts for each node/edge.
         */
        std::vector<std::vector<unsigned int> > counts;
        /**
         * @brief Suppress verbose output if true.
         */
        bool quiet = false;
        /**
         * @brief List of significant patterns discovered in the graph.
         */
        std::vector<SignificantPattern> significant_patterns;
        /**
         * @brief Number of rewiring operations performed.
         */
        unsigned int rewiring_ops = 0;

        // Internal graph construction and pattern discovery methods
        void buildInitialGraph(const std::vector<std::vector<std::string> > &sequences);
        bool distill(const SearchPath &search_path, const ADIOSParams &params);
        bool generalise(const SearchPath &search_path, const ADIOSParams &params);

        // Pattern generalization and bootstrapping
        EquivalenceClass computeEquivalenceClass(const SearchPath &search_path, unsigned int slotIndex) const;
        SearchPath bootstrap(std::vector<EquivalenceClass> &encountered_ecs, const SearchPath &search_path, double overlapThreshold) const;

        // Matrix computation and pattern search
        void computeConnectionMatrix(ConnectionMatrix &connections, const SearchPath &search_path) const;
        void computeDescentsMatrix(TNT::Array2D<double> &flows, TNT::Array2D<double> &descents, const ConnectionMatrix &connections) const;
        bool findSignificantPatterns(std::vector<Range> &patterns, std::vector<SignificancePair> &pvalues, const ConnectionMatrix &connections, const TNT::Array2D<double> &flows, const TNT::Array2D<double> &descents, double eta, double alpha) const;

        // Rewiring and update functions
        void updateAllConnections();
        void rewire(const std::vector<Connection> &connections, unsigned int ec);
        void rewire(const std::vector<Connection> &connections, const EquivalenceClass &ec);
        void rewire(const std::vector<Connection> &connections, const SignificantPattern &sp);
        std::vector<Connection> getRewirableConnections(const ConnectionMatrix &connections, const Range &bestSP, double alpha) const;
        double computeRightSignificance(const ConnectionMatrix &connections, const TNT::Array2D<double> &flows, const std::pair<unsigned int, unsigned int> &descentPoint, double eta) const;
        double computeLeftSignificance(const ConnectionMatrix &connections, const TNT::Array2D<double> &flows, const std::pair<unsigned int, unsigned int> &descentPoint, double eta) const;
        double findBestRightDescentColumn(unsigned int &bestColumn, TNT::Array2D<double> &pvalueCache, const ConnectionMatrix &connections, const TNT::Array2D<double> &flows, const TNT::Array2D<double> &descents, const Range &pattern, double eta) const;
        double findBestLeftDescentColumn(unsigned int &bestColumn, TNT::Array2D<double> &pvalueCache, const ConnectionMatrix &connections, const TNT::Array2D<double> &flows, const TNT::Array2D<double> &descents, const Range &pattern, double eta) const;

        // Auxiliary functions
        std::vector<Connection> filterConnections(const std::vector<Connection> &init_cons, unsigned int start_offset, const SearchPath &search_path) const;
        std::vector<Connection> getAllNodeConnections(unsigned int nodeIndex) const;
        unsigned int findExistingEquivalenceClass(const EquivalenceClass &ec) const;

        // Counts the occurrences of each lexicon unit
        void estimateProbabilities();

        // Print functions
        std::string printSignificantPattern(const SignificantPattern &sp) const;
        std::string printEquivalenceClass(const EquivalenceClass &ec) const;
        std::string printNode(unsigned int node) const;
        std::string printPath(const SearchPath &path) const;
        std::string printNodeName(unsigned int node) const;
};

/**
 * @brief Print information about the connection, flow, and descent matrices (for debugging).
 * @param connections The connection matrix.
 * @param flows The flow matrix.
 * @param descents The descent matrix.
 */
void printInfo(const ConnectionMatrix &connections, const TNT::Array2D<double> &flows, const TNT::Array2D<double> &descents);

#endif
