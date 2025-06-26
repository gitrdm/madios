/**
 * @file EquivalenceClass.h
 * @brief Declares the EquivalenceClass class for grouping similar patterns or nodes in the ADIOS algorithm.
 *
 * Inherits from LexiconUnit and std::vector<unsigned int>.
 */
#pragma once

#ifndef EQUIVALENCECLASS_H
#define EQUIVALENCECLASS_H

#include "LexiconUnit.h"
#include <vector>
#include <string>
#include <sstream>

/**
 * @class EquivalenceClass
 * @brief Groups similar patterns or nodes for generalization in the ADIOS algorithm.
 *
 * Inherits from LexiconUnit and std::vector<unsigned int>.
 */
class EquivalenceClass: public LexiconUnit, public std::vector<unsigned int>
{
    public:
        /**
         * @brief Default constructor. Creates an empty equivalence class.
         */
        EquivalenceClass();
        /**
         * @brief Construct from a vector of unit indices.
         * @param units Vector of unit indices to initialize the class.
         */
        explicit EquivalenceClass(const std::vector<unsigned int> &units);
        /**
         * @brief Destructor.
         */
        virtual ~EquivalenceClass();
        /**
         * @brief Compute the overlap with another equivalence class.
         * @param other The other equivalence class.
         * @return A new EquivalenceClass representing the overlap.
         */
        EquivalenceClass computeOverlapEC(const EquivalenceClass &other) const;
        /**
         * @brief Check if the class contains a unit.
         * @param unit The unit index to check.
         * @return True if present, false otherwise.
         */
        bool has(unsigned int unit) const;
        /**
         * @brief Add a unit to the class.
         * @param unit The unit index to add.
         * @return True if added, false if already present.
         */
        bool add(unsigned int unit);
        /**
         * @brief Make a copy of this equivalence class (polymorphic clone).
         * @return Pointer to a new LexiconUnit copy.
         */
        virtual LexiconUnit* makeCopy() const;
        /**
         * @brief Get a string representation of the equivalence class.
         * @return String describing the contents.
         */
        virtual std::string toString() const;
};

#endif
