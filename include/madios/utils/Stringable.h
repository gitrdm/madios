// File: utils/Stringable.h
// Purpose: Declares the Stringable class, which provides string conversion utilities for the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.

#ifndef STRINGABLE_H
#define STRINGABLE_H

#include <string>

/**
 * @class Stringable
 * @brief Abstract base class providing string conversion utilities.
 *
 * Classes inheriting from Stringable must implement toString().
 */
class Stringable
{
    public:
        /**
         * @brief Default constructor.
         */
        Stringable();
        /**
         * @brief Virtual destructor.
         */
        virtual ~Stringable();
        /**
         * @brief Get a string representation of the object.
         * @return String describing the object.
         */
        virtual std::string toString() const = 0;
        /**
         * @brief Output stream operator for Stringable objects.
         * @param out Output stream.
         * @param Stringable The object to print.
         * @return Reference to the output stream.
         */
        friend std::ostream& operator<<(std::ostream &out, const Stringable &Stringable);
};

#endif
