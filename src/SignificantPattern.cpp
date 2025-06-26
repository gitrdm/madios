// File: SignificantPattern.cpp
// Purpose: Implements the SignificantPattern class, which detects and manages significant patterns in the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.
//
// Major responsibilities:
//   - Represent a significant pattern (sequence of node indices) in the ADIOS graph
//   - Provide robust copying, search, and string conversion
//   - Used for pattern discovery and generalization
//
// Design notes:
//   - Inherits from LexiconUnit and std::vector<unsigned int>
//   - All methods are robust to empty and duplicate input
//   - Used throughout the ADIOS algorithm for significant pattern management

#include "SignificantPattern.h"
#include "madios/Logger.h"
#include "madios/BasicSymbol.h"

#include <cassert>

using std::vector;
using std::string;
using std::ostringstream;

/**
 * @brief Default constructor. Initializes an empty significant pattern.
 */
SignificantPattern::SignificantPattern()
{
    madios::Logger::trace("SignificantPattern default constructor");
}

/**
 * @brief Construct a significant pattern from a vector of node indices.
 * @param sequence Vector of node indices
 */
SignificantPattern::SignificantPattern(const vector<unsigned int> &sequence)
{
    if (sequence.empty()) {
        throw std::invalid_argument("SignificantPattern: input sequence vector is empty");
    }
    madios::Logger::trace("SignificantPattern constructed from vector, size: " + std::to_string(sequence.size()));
    clear();
    for(unsigned int i = 0; i < sequence.size(); i++)
        push_back(sequence[i]);
}

/**
 * @brief Destructor. No special cleanup needed.
 */
SignificantPattern::~SignificantPattern()
{
}

/**
 * @brief Find the index of a unit in the pattern.
 * @param unit Node index to find
 * @return Index of the unit in the pattern
 * @throws Asserts if not found
 */
unsigned int SignificantPattern::find(unsigned int unit) const
{
    madios::Logger::trace("SignificantPattern::find(" + std::to_string(unit) + ") called");
    for(unsigned int i = 0; i < size(); i++)
        if(at(i) == unit)
            return i;
    madios::Logger::error("SignificantPattern::find(" + std::to_string(unit) + ") not found, throwing exception");
    throw std::out_of_range("SignificantPattern::find: unit not found in pattern");
}

/**
 * @brief Make a deep copy of this significant pattern.
 * @return Pointer to a new SignificantPattern object
 */
LexiconUnit* SignificantPattern::makeCopy() const
{
    madios::Logger::trace("SignificantPattern::makeCopy() called");
    return new SignificantPattern(*this);
}

/**
 * @brief Convert the significant pattern to a human-readable string.
 * @return String representation of the pattern
 */
string SignificantPattern::toString() const
{
    madios::Logger::trace("SignificantPattern::toString() called");
    ostringstream sout;

    sout << "P[";
    if(size() > 0)
    {
        for(unsigned int i = 0; i < size() - 1; i++)
            sout << "P" << at(i) << " - ";
        if(size() > 0) sout << "P" << back();
    }
    sout << "]";

    return sout.str();
}
