// File: BasicSymbol.h
// Purpose: Declares the BasicSymbol class, which handles basic symbols (tokens/words) in the corpus for the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#ifndef MADIOS_BASIC_SYMBOL_H
#define MADIOS_BASIC_SYMBOL_H

#include "LexiconUnit.h"
#include <string>

class BasicSymbol : public LexiconUnit {
public:
    BasicSymbol();
    explicit BasicSymbol(const std::string &symbol);
    virtual ~BasicSymbol();

    LexiconUnit* makeCopy() const override;
    std::string toString() const override;
    std::string getSymbol() const;

    bool operator==(const BasicSymbol &other) const { return symbol == other.symbol; }

private:
    std::string symbol;
};

#endif // MADIOS_BASIC_SYMBOL_H
