/**
 * @file ADIOSUtils.h
 * @brief Utilities and types for the ADIOS algorithm, including parameters and lexicon types.
 */
#pragma once

#ifndef ADIOSUTILS_H
#define ADIOSUTILS_H

#include "BasicSymbol.h"
#include "SpecialLexicons.h"
#include "SignificantPattern.h"
#include "EquivalenceClass.h"
#include "SearchPath.h"

/**
 * @class ADIOSParams
 * @brief Parameter set for the ADIOS algorithm.
 *
 * Holds configuration values such as eta, alpha, context size, and overlap threshold.
 */
class ADIOSParams
{
    public:
        double eta;               ///< Eta parameter for ADIOS.
        double alpha;             ///< Alpha parameter for ADIOS.
        unsigned int contextSize; ///< Context size parameter.
        double overlapThreshold;  ///< Overlap threshold parameter.

        /**
         * @brief Construct ADIOSParams with all parameters specified.
         * @param eta Eta parameter.
         * @param alpha Alpha parameter.
         * @param contextSize Context size parameter.
         * @param overlapThreshold Overlap threshold parameter.
         */
        ADIOSParams(double eta, double alpha, unsigned int contextSize, double overlapThreshold);
};

/**
 * @namespace LexiconTypes
 * @brief Contains enumeration for lexicon entry types used in ADIOS.
 */
namespace LexiconTypes
{
/**
 * @enum LexiconEnum
 * @brief Types of lexicon entries in ADIOS.
 */
enum LexiconEnum
{
    Start,   ///< Start symbol
    End,     ///< End symbol
    Symbol,  ///< Basic symbol
    SP,      ///< Significant pattern
    EC       ///< Equivalence class
};
}

#endif
