// File: SpecialLexicons.h
// Purpose: Declares the SpecialLexicons class, which handles special lexicon entries for the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#ifndef SPECIALLEXICONS_H
#define SPECIALLEXICONS_H

#include "LexiconUnit.h"

#include <string>

class StartSymbol: public LexiconUnit
{
    public:
        virtual LexiconUnit* makeCopy() const;
        virtual std::string toString() const;
};

class EndSymbol: public LexiconUnit
{
    public:
        virtual LexiconUnit* makeCopy() const;
        virtual std::string toString() const;
};

#endif
