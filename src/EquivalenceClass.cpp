// File: EquivalenceClass.cpp
// Purpose: Implements the EquivalenceClass class, which groups similar patterns or nodes for generalization in the ADIOS algorithm.
// Part of the ADIOS grammar induction project. See README for usage and structure.
//
// Major responsibilities:
//   - Represent an equivalence class of nodes or patterns for generalization
//   - Provide set-like operations (add, has, overlap)
//   - Support robust copying and string conversion for debugging/output
//
// Design notes:
//   - Inherits from LexiconUnit and std::vector<unsigned int>
//   - All methods are robust to empty and duplicate input
//   - Used throughout the ADIOS algorithm for pattern generalization

#include "EquivalenceClass.h"
#include "madios/Logger.h"
#include "madios/BasicSymbol.h"

#include <algorithm>

using std::vector;
using std::string;
using std::ostringstream;

/**
 * @brief Default constructor. Initializes an empty equivalence class.
 */
EquivalenceClass::EquivalenceClass()
{
    madios::Logger::trace("EquivalenceClass default constructor");
}

/**
 * @brief Construct an equivalence class from a vector of units.
 * @param units Vector of node indices to include in the class
 */
EquivalenceClass::EquivalenceClass(const vector<unsigned int> &units)
:vector<unsigned int>(units)
{
    madios::Logger::trace("EquivalenceClass constructed from vector, size: " + std::to_string(units.size()));
}

/**
 * @brief Destructor. No special cleanup needed.
 */
EquivalenceClass::~EquivalenceClass()
{
}

/**
 * @brief Compute the overlap (intersection) with another equivalence class.
 * @param other The other equivalence class
 * @return An equivalence class containing only the shared units
 */
EquivalenceClass EquivalenceClass::computeOverlapEC(const EquivalenceClass &other) const
{
    EquivalenceClass overlap;
    for(unsigned int i = 0; i < other.size(); i++)
        if(has(other[i]))
            overlap.add(other[i]);

    return overlap;
}

/**
 * @brief Check if a unit is present in the equivalence class.
 * @param unit Node index to check
 * @return True if present, false otherwise
 */
bool EquivalenceClass::has(unsigned int unit) const
{
    bool present = (find(begin(), end(), unit) != end());
    madios::Logger::trace("EquivalenceClass::has(" + std::to_string(unit) + ") => " + (present ? "true" : "false"));
    return present;
}

/**
 * @brief Add a unit to the equivalence class if not already present.
 * @param unit Node index to add
 * @return True if added, false if already present
 */
bool EquivalenceClass::add(unsigned int unit)
{
    if(has(unit)) {
        madios::Logger::trace("EquivalenceClass::add(" + std::to_string(unit) + ") skipped (already present)");
        return false;
    }
    push_back(unit);
    madios::Logger::trace("EquivalenceClass::add(" + std::to_string(unit) + ") added");
    return true;
}

/**
 * @brief Make a deep copy of this equivalence class.
 * @return Pointer to a new EquivalenceClass object
 */
LexiconUnit* EquivalenceClass::makeCopy() const
{
    madios::Logger::trace("EquivalenceClass::makeCopy() called");
    return new EquivalenceClass(*this);
}

/**
 * @brief Convert the equivalence class to a human-readable string.
 * @return String representation of the class
 */
string EquivalenceClass::toString() const
{
    madios::Logger::trace("EquivalenceClass::toString() called");
    ostringstream sout;

    sout << "E[";
    if(size() > 0)
    {
        for(unsigned int i = 0; i < size() - 1; i++)
            sout << "P" << at(i) << " | ";
        if(size() > 0) sout << "P" << back();
    }
    sout << "]";

    return sout.str();
}
