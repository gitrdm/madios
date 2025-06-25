// File: utils/Stringable.h
// Purpose: Declares the Stringable class, which provides string conversion utilities for the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#ifndef STRINGABLE_H
#define STRINGABLE_H

#include <string>

class Stringable
{
    public:
        Stringable();
        virtual ~Stringable();

        virtual std::string toString() const = 0;
        friend std::ostream& operator<<(std::ostream &out, const Stringable &Stringable);
};

#endif
