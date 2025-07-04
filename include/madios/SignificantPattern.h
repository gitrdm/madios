/**
 * @file SignificantPattern.h
 * @brief Declares the SignificantPattern class for detecting and managing significant patterns in the ADIOS algorithm.
 *
 * The SignificantPattern class inherits from LexiconUnit and std::vector<unsigned int>
 * and provides methods for pattern management in the ADIOS algorithm.
 */
#pragma once

#ifndef SIGNIFICANTPATTERN_H
#define SIGNIFICANTPATTERN_H

#include "LexiconUnit.h"
#include <vector>
#include <sstream>

/**
 * @class SignificantPattern
 * @brief Detects and manages significant patterns in the ADIOS algorithm.
 *
 * Inherits from LexiconUnit and std::vector<unsigned int>.
 * Provides methods for construction, copying, searching, and string representation.
 */
class SignificantPattern: public LexiconUnit, public std::vector<unsigned int>
{
    public:
        /**
         * @brief Default constructor. Creates an empty significant pattern.
         */
        SignificantPattern();
        /**
         * @brief Constructs a significant pattern from a sequence of unit indices.
         * @param sequence Vector of unit indices to initialize the pattern.
         */
        explicit SignificantPattern(const std::vector<unsigned int> &sequence);
        /**
         * @brief Destructor.
         */
        virtual ~SignificantPattern();
        /**
         * @brief Find the index of a unit in the pattern.
         * @param unit The unit index to find.
         * @return The index if found, or std::vector<unsigned int>::npos if not found.
         */
        unsigned int find(unsigned int unit) const;
        /**
         * @brief Make a copy of this significant pattern (polymorphic clone).
         * @return Pointer to a new LexiconUnit copy.
         */
        virtual LexiconUnit* makeCopy() const;
        /**
         * @brief Get a string representation of the significant pattern.
         * @return String describing the contents.
         */
        virtual std::string toString() const;
};

#endif
