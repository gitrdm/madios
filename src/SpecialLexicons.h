// File: SpecialLexicons.h
// Purpose: Declares special lexicon unit classes (StartSymbol, EndSymbol) for the ADIOS grammar induction algorithm.
// These classes represent special boundary markers in the input sequence graph.
//
// Part of the ADIOS grammar induction project. See README for usage and structure.
//
// Design:
//   - StartSymbol and EndSymbol inherit from LexiconUnit and represent the start and end of a sequence, respectively.
//   - These are used as unique markers in the graph to denote sequence boundaries.
//   - Both provide copy and string conversion methods for use in graph construction and output.

#ifndef SPECIALLEXICONS_H
#define SPECIALLEXICONS_H

#include "LexiconUnit.h"
#include <string>

/**
 * @class StartSymbol
 * @brief Represents the special start-of-sequence marker in the ADIOS graph.
 * Inherits from LexiconUnit. Used to mark the beginning of a sequence.
 */
class StartSymbol : public LexiconUnit {
public:
    /// Default constructor
    StartSymbol() = default;
    /// Default destructor
    ~StartSymbol() override = default;
    /// Copy constructor
    StartSymbol(const StartSymbol&) = default;
    /// Assignment operator
    StartSymbol& operator=(const StartSymbol&) = default;

    /**
     * @brief Create a heap-allocated copy of this StartSymbol.
     * @return LexiconUnit* pointer to a new StartSymbol instance.
     */
    LexiconUnit* makeCopy() const override;

    /**
     * @brief Get a string representation of the start symbol.
     * @return std::string "START"
     */
    std::string toString() const override;
};

/**
 * @class EndSymbol
 * @brief Represents the special end-of-sequence marker in the ADIOS graph.
 * Inherits from LexiconUnit. Used to mark the end of a sequence.
 */
class EndSymbol : public LexiconUnit {
public:
    /// Default constructor
    EndSymbol() = default;
    /// Default destructor
    ~EndSymbol() override = default;
    /// Copy constructor
    EndSymbol(const EndSymbol&) = default;
    /// Assignment operator
    EndSymbol& operator=(const EndSymbol&) = default;

    /**
     * @brief Create a heap-allocated copy of this EndSymbol.
     * @return LexiconUnit* pointer to a new EndSymbol instance.
     */
    LexiconUnit* makeCopy() const override;

    /**
     * @brief Get a string representation of the end symbol.
     * @return std::string "END"
     */
    std::string toString() const override;
};

#endif // SPECIALLEXICONS_H
