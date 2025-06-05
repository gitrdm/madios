// File: EquivalenceClass.h
// Purpose: Declares the EquivalenceClass class, which groups similar patterns or nodes for generalization in the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#ifndef EQUIVALENCECLASS_H
#define EQUIVALENCECLASS_H

#include "LexiconUnit.h"

#include <vector>
#include <string>
#include <sstream>

class EquivalenceClass: public LexiconUnit, public std::vector<unsigned int>
{
    public:
        EquivalenceClass();
        explicit EquivalenceClass(const std::vector<unsigned int> &units);
        virtual ~EquivalenceClass();

        EquivalenceClass computeOverlapEC(const EquivalenceClass &other) const;
        bool has(unsigned int unit) const;
        bool add(unsigned int unit);

        virtual LexiconUnit* makeCopy() const;
        virtual std::string toString() const;
};

#endif
