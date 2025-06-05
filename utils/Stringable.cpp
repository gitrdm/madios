// File: utils/Stringable.cpp
// Purpose: Implements the Stringable class, which provides string conversion utilities for the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#include "Stringable.h"

using std::ostream;

Stringable::Stringable()
{
}

Stringable::~Stringable()
{
}

ostream& operator<<(ostream &out, const Stringable &stringable)
{
    out << stringable.toString();
    return out;
}
