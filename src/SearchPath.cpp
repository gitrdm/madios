// File: SearchPath.cpp
// Purpose: Implements the SearchPath class, which manages search paths through the ADIOS graph for parsing or pattern finding.
// Part of the ADIOS grammar induction project. See README for usage and structure.
//
// Major responsibilities:
//   - Represent a path (sequence of node indices) through the ADIOS graph
//   - Support path manipulation, comparison, and rewiring
//   - Used for parsing, pattern finding, and generalization
//
// Design notes:
//   - Inherits from std::vector<unsigned int>
//   - All methods are robust to empty and out-of-bounds input
//   - Used throughout the ADIOS algorithm for search and pattern management

#include "SearchPath.h"
#include "madios/BasicSymbol.h"

#include <cassert>

using std::string;
using std::vector;
using std::ostream;
using std::ostringstream;
using std::endl;

/**
 * @brief Default constructor. Initializes an empty search path.
 */
SearchPath::SearchPath()
{
}

/**
 * @brief Construct a search path from a vector of node indices.
 * @param path Vector of node indices
 */
SearchPath::SearchPath(const vector<unsigned int> &path)
:vector<unsigned int>(path)
{
}

/**
 * @brief Destructor. No special cleanup needed.
 */
SearchPath::~SearchPath()
{
}

/**
 * @brief Equality comparison operator.
 * @param other SearchPath to compare with
 * @return True if paths are identical, false otherwise
 */
bool SearchPath::operator==(const SearchPath &other) const
{
    if(size() != other.size())
        return false;

    for(unsigned int i = 0; i < size(); i++)
        if(at(i) != other[i])
            return false;

    return true;
}

/**
 * @brief Rewire a segment of the path to a new node.
 * @param start Start index (inclusive)
 * @param finish End index (inclusive)
 * @param node Node index to insert
 */
void SearchPath::rewire(unsigned int start, unsigned int finish, unsigned int node)
{
    erase( begin()+start, begin()+finish+1);
    insert(begin()+start, node);
}

/**
 * @brief Extract a subpath from start to finish (inclusive).
 * @param start Start index
 * @param finish End index
 * @return Vector of node indices in the subpath
 */
vector<unsigned int> SearchPath::operator()(unsigned int start, unsigned int finish) const
{
    assert(start <= finish);
    assert(finish< size());

    return vector<unsigned int>(begin()+start, begin()+finish+1);
}

/**
 * @brief Substitute a segment of the path with a new sequence of nodes.
 * @param start Start index of the segment to replace
 * @param finish End index of the segment to replace
 * @param segment Vector of new node indices to insert
 * @return A new SearchPath with the substituted segment
 */
vector<unsigned int> SearchPath::substitute(unsigned int start, unsigned int finish, const vector<unsigned int> &segment) const
{
    assert(start <= finish);
    assert(finish < size());

    vector<unsigned int> new_path(begin(), begin()+start);
    new_path.insert(new_path.end(), segment.begin(), segment.end());
    new_path.insert(new_path.end(), begin()+finish+1, end());

    return new_path;
}

/**
 * @brief Convert the search path to a string representation.
 * @return String representation of the search path
 */
string SearchPath::toString() const
{
    ostringstream sout;
    sout << "[";
    for(unsigned int i = 0; i < size() - 1; i++)
        sout << at(i) << " --> ";
    sout << back() << "]";

    return sout.str();
}
