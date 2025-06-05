// File: SpecialLexicons.cpp
// Purpose: Implements the SpecialLexicons class, which handles special lexicon entries for the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#include "SpecialLexicons.h"

using std::string;

LexiconUnit* StartSymbol::makeCopy() const
{
    return new StartSymbol();
}

string StartSymbol::toString() const
{
    return "START";
}

LexiconUnit* EndSymbol::makeCopy() const
{
    return new EndSymbol();
}

string EndSymbol::toString() const
{
    return "END";
}
