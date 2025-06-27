// File: RDSNode.cpp
// Purpose: Implements the RDSNode class, representing nodes (words or patterns) in the ADIOS graph.
// Part of the ADIOS grammar induction project. See README for usage and structure.
//
// Major responsibilities:
//   - Represent a node in the ADIOS graph (word, pattern, or equivalence class)
//   - Manage connections and parent relationships
//   - Ensure safe memory management for owned LexiconUnit pointers
//   - Provide robust copy and assignment semantics
//
// Design notes:
//   - Each RDSNode owns its LexiconUnit pointer and is responsible for deletion
//   - Copy and assignment perform deep copies of LexiconUnit and all relationships
//   - Defensive guards prevent double deletion and ensure valid state

#include "RDSNode.h"
#include "madios/BasicSymbol.h"
#include <utility>

using std::vector;

/**
 * @brief Default constructor. Initializes an empty node with no lexicon.
 */
RDSNode::RDSNode()
{
    this->lexicon = nullptr;
}

/**
 * @brief Construct a node with a given lexicon and type.
 * @param lexicon Pointer to a LexiconUnit (ownership is taken)
 * @param type Lexicon type (Symbol, EC, SP, etc.)
 */
RDSNode::RDSNode(std::unique_ptr<LexiconUnit> lexicon, LexiconTypes::LexiconEnum type)
{
    if (!lexicon) {
        throw std::invalid_argument("RDSNode: lexicon pointer is null");
    }
    this->lexicon = std::move(lexicon);
    this->type = type;
}

/**
 * @brief Copy constructor. Performs a deep copy of the node and its relationships.
 * @param other Node to copy from
 */
RDSNode::RDSNode(const RDSNode &other)
{
    lexicon = other.lexicon ? std::unique_ptr<LexiconUnit>(other.lexicon->makeCopy()) : nullptr;
    type = other.type;
    connections = other.connections;
    parents = other.parents;
}

/**
 * @brief Destructor. Deletes the owned LexiconUnit pointer.
 */
RDSNode::~RDSNode()
{
    // unique_ptr handles deletion
    lexicon = nullptr;
}

/**
 * @brief Assignment operator. Performs a deep copy and manages memory safely.
 * @param other Node to assign from
 * @return Reference to this node
 */
RDSNode& RDSNode::operator=(const RDSNode &other)
{
    if (this != &other) {
        lexicon.reset(other.lexicon ? other.lexicon->makeCopy() : nullptr);
        type = other.type;
        connections = other.connections;
        parents = other.parents;
    }
    return *this;
}

/**
 * @brief Add a connection to this node.
 * @param con Connection to add
 */
void RDSNode::addConnection(const Connection &con)
{
    // Defensive: check for valid connection (could add more checks if needed)
    if (con.first == static_cast<unsigned int>(-1) || con.second == static_cast<unsigned int>(-1)) {
        throw std::invalid_argument("RDSNode::addConnection: invalid connection indices");
    }
    connections.push_back(con);
}

/**
 * @brief Get all connections from this node.
 * @return std::vector of connections
 */
const std::vector<Connection>& RDSNode::getConnections() const
{
    return connections;
}

/**
 * @brief Set the connections for this node.
 * @param connections New std::vector of connections
 */
void RDSNode::setConnections(const std::vector<Connection> &connections)
{
    this->connections = connections;
}

/**
 * @brief Add a parent connection if not already present.
 * @param newParent Connection to add
 * @return True if added, false if already present
 */
bool RDSNode::addParent(const Connection &newParent)
{
    for(const auto& parent : parents)
        if(parent == newParent)
            return false;
    if (newParent.first == static_cast<unsigned int>(-1) || newParent.second == static_cast<unsigned int>(-1)) {
        throw std::invalid_argument("RDSNode::addParent: invalid parent connection indices");
    }
    parents.push_back(newParent);
    return true;
}

/**
 * @brief Deep copy helper for copy constructor and assignment.
 * @param other Node to copy from
 */
void RDSNode::deepCopy(const RDSNode &other)
{
    lexicon.reset(other.lexicon ? other.lexicon->makeCopy() : nullptr);
    type = other.type;
    connections = other.connections;
    parents = other.parents;
}
