/**
 * @file BasicSymbol.h
 * @brief Declares the BasicSymbol class for handling basic symbols in the ADIOS algorithm.
 *
 * Inherits from LexiconUnit.
 */
#pragma once

#ifndef BASICSYMBOL_H
#define BASICSYMBOL_H

#include "LexiconUnit.h"

/**
 * @class BasicSymbol
 * @brief Handles basic symbols (tokens/words) in the corpus for the ADIOS algorithm.
 *
 * Inherits from LexiconUnit.
 */
class BasicSymbol: public LexiconUnit
{
    public:
        /**
         * @brief Default constructor. Creates an empty symbol.
         */
        BasicSymbol();
        /**
         * @brief Construct from a string symbol.
         * @param symbol The symbol to store.
         */
        explicit BasicSymbol(const std::string &symbol);
        /**
         * @brief Destructor.
         */
        virtual ~BasicSymbol();
        /**
         * @brief Make a copy of this symbol (polymorphic clone).
         * @return Pointer to a new LexiconUnit copy.
         */
        virtual LexiconUnit* makeCopy() const;
        /**
         * @brief Get a string representation of the symbol.
         * @return String describing the symbol.
         */
        virtual std::string toString() const;
        /**
         * @brief Get the stored symbol string.
         * @return The symbol as a string.
         */
        std::string getSymbol() const;
        /**
         * @brief Equality operator.
         * @param other The other BasicSymbol to compare.
         * @return True if equal, false otherwise.
         */
        bool operator==(const BasicSymbol& other) const { return symbol == other.symbol; }

    private:
        std::string symbol; ///< The stored symbol string
};

#endif
