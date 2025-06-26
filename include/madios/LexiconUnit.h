/**
 * @file LexiconUnit.h
 * @brief Declares the LexiconUnit abstract base class for lexicon units in the ADIOS algorithm.
 */
#pragma once

#ifndef LEXICONUNIT_H
#define LEXICONUNIT_H

#include "utils/Stringable.h"

/**
 * @class LexiconUnit
 * @brief Abstract base class for lexicon units in the ADIOS algorithm.
 *
 * Provides a polymorphic interface for copying and string conversion.
 */
class LexiconUnit: public Stringable
{
    public:
        /**
         * @brief Polymorphic copy constructor.
         * @return Pointer to a new LexiconUnit copy.
         */
        virtual LexiconUnit* makeCopy() const = 0;
};

#endif
