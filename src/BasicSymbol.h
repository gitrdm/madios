// File: BasicSymbol.h
// Purpose: Declares the BasicSymbol class, which handles basic symbols (tokens/words) in the corpus for the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#ifndef BASICSYMBOL_H
#define BASICSYMBOL_H

#include "LexiconUnit.h"

class BasicSymbol: public LexiconUnit
{
    public:
        BasicSymbol();
        explicit BasicSymbol(const std::string &symbol);
        virtual ~BasicSymbol();

        virtual LexiconUnit* makeCopy() const;
        virtual std::string toString() const;
        std::string getSymbol() const;

    private:
        std::string symbol;
};

#endif
