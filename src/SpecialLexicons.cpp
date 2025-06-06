// File: SpecialLexicons.cpp
// Purpose: Implements special lexicon unit classes (StartSymbol, EndSymbol) for the ADIOS grammar induction algorithm.
// These classes represent special boundary markers in the input sequence graph.
//
// Part of the ADIOS grammar induction project. See README for usage and structure.
//
// Design:
//   - StartSymbol and EndSymbol inherit from LexiconUnit and represent the start and end of a sequence, respectively.
//   - Both provide copy and string conversion methods for use in graph construction and output.

#include "SpecialLexicons.h"
#include <stdexcept>
#include <string>

using std::string;

/**
 * @brief Create a heap-allocated copy of this StartSymbol.
 * @return LexiconUnit* pointer to a new StartSymbol instance.
 */
LexiconUnit* StartSymbol::makeCopy() const {
    // Defensive: If extended, ensure correct type
    return new StartSymbol(*this);
}

/**
 * @brief Get a string representation of the start symbol.
 * @return std::string "START"
 */
string StartSymbol::toString() const {
    return "START";
}

/**
 * @brief Create a heap-allocated copy of this EndSymbol.
 * @return LexiconUnit* pointer to a new EndSymbol instance.
 */
LexiconUnit* EndSymbol::makeCopy() const {
    // Defensive: If extended, ensure correct type
    return new EndSymbol(*this);
}

/**
 * @brief Get a string representation of the end symbol.
 * @return std::string "END"
 */
string EndSymbol::toString() const {
    return "END";
}
