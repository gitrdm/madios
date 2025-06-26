// File: BasicSymbol.cpp
// Purpose: Implements the BasicSymbol class, which handles basic symbols (tokens/words) in the corpus for the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.
//
// Major responsibilities:
//   - Represent a basic symbol (token/word) in the ADIOS graph
//   - Provide copying and string conversion utilities
//
// Design notes:
//   - Inherits from LexiconUnit
//   - Used throughout the ADIOS algorithm for representing atomic units

#include "BasicSymbol.h"

using std::string;
using std::ostream;

/**
 * @brief Default constructor. Initializes an empty symbol.
 */
BasicSymbol::BasicSymbol()
{
}

/**
 * @brief Construct a BasicSymbol from a string.
 * @param symbol The symbol string to store
 */
BasicSymbol::BasicSymbol(const string &symbol)
{
    this->symbol = symbol;
}

/**
 * @brief Destructor. No special cleanup needed.
 */
BasicSymbol::~BasicSymbol()
{
}

/**
 * @brief Make a copy of this BasicSymbol.
 * @return Pointer to a new BasicSymbol copy
 */
LexiconUnit* BasicSymbol::makeCopy() const
{
    return new BasicSymbol(*this);
}

/**
 * @brief Get the string representation of the symbol.
 * @return The symbol as a string
 */
string BasicSymbol::toString() const
{
    return symbol;
}

/**
 * @brief Get the symbol string.
 * @return The symbol as a string
 */
string BasicSymbol::getSymbol() const
{
    return symbol;
}
