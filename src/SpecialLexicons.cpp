// File: SpecialLexicons.cpp
// Purpose: Implements special lexicon unit classes (StartSymbol, EndSymbol) for the ADIOS grammar induction algorithm.
// These classes represent special boundary markers in the input sequence graph.
//
// Part of the ADIOS grammar induction project. See README for usage and structure.
//
// Design:
//   - StartSymbol and EndSymbol inherit from LexiconUnit and represent the start and end of a sequence, respectively.
//   - Both provide copy and string conversion methods for use in graph construction and output.
//
// For a novice C++ programmer: These classes are simple wrappers for special tokens used to mark the beginning and end of input sequences.
// They are used throughout the graph to ensure correct parsing and output.

#include "madios/BasicSymbol.h"
#include "SpecialLexicons.h"
#include <stdexcept>
#include <string>

using std::string;

/**
 * @class StartSymbol
 * @brief Special lexicon unit representing the start of a sequence.
 * Inherits from LexiconUnit.
 */
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
 * @class EndSymbol
 * @brief Special lexicon unit representing the end of a sequence.
 * Inherits from LexiconUnit.
 */
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
