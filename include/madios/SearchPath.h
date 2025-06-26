/**
 * @file SearchPath.h
 * @brief Declares the SearchPath class for managing search paths in the ADIOS algorithm.
 *
 * Inherits from Stringable and std::vector<unsigned int>.
 */
#pragma once

#ifndef SEARCHPATH_H
#define SEARCHPATH_H

#include "utils/Stringable.h"
#include <fstream>
#include <sstream>
#include <vector>

/**
 * @class SearchPath
 * @brief Manages search paths through the ADIOS graph for parsing or pattern finding.
 *
 * Inherits from Stringable and std::vector<unsigned int>.
 */
class SearchPath: public Stringable, public std::vector<unsigned int>
{
    public:
        /**
         * @brief Default constructor. Creates an empty search path.
         */
        SearchPath();
        /**
         * @brief Construct from a vector of node indices.
         * @param path Vector of node indices to initialize the path.
         */
        explicit SearchPath(const std::vector<unsigned int> &path);
        /**
         * @brief Destructor.
         */
        virtual ~SearchPath();
        /**
         * @brief Equality operator.
         * @param other The other SearchPath to compare.
         * @return True if equal, false otherwise.
         */
        bool operator==(const SearchPath &other) const;
        /**
         * @brief Rewire a segment of the path to a new node.
         * @param start Start index.
         * @param finish End index.
         * @param node Node to insert.
         */
        void rewire(unsigned int start, unsigned int finish, unsigned int node);
        /**
         * @brief Get a subpath from start to finish (inclusive).
         * @param start Start index.
         * @param finish End index.
         * @return Vector of node indices in the subpath.
         */
        std::vector<unsigned int> operator()(unsigned int start, unsigned int finish) const;
        /**
         * @brief Substitute a segment of the path with a new segment.
         * @param start Start index.
         * @param finish End index.
         * @param segment The segment to insert.
         * @return New vector with the substitution applied.
         */
        std::vector<unsigned int> substitute(unsigned int start, unsigned int finish, const std::vector<unsigned int> &segment) const;
        /**
         * @brief Get a string representation of the search path.
         * @return String describing the path.
         */
        virtual std::string toString() const;
};

#endif
