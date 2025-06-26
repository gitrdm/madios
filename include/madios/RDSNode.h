/**
 * @file RDSNode.h
 * @brief Declares the RDSNode class representing nodes in the ADIOS graph.
 *
 * Part of the ADIOS grammar induction project. See README for usage and structure.
 */
#pragma once

#ifndef RDSNODE_H
#define RDSNODE_H

#include "madios/maths/tnt/array2d.h"
#include "LexiconUnit.h"
#include "ADIOSUtils.h"
#include <memory>

/**
 * @typedef Connection
 * @brief Represents a connection as a pair of unsigned integers (from, to).
 */
typedef std::pair<unsigned int, unsigned int> Connection;
/**
 * @typedef ConnectionMatrix
 * @brief Matrix of connections for the ADIOS graph.
 */
typedef TNT::Array2D<std::vector<Connection> > ConnectionMatrix;
/**
 * @typedef SignificancePair
 * @brief Pair of significance values (left, right).
 */
typedef std::pair<double, double> SignificancePair;
/**
 * @typedef Range
 * @brief Represents a range as a pair of unsigned integers (start, end).
 */
typedef std::pair<unsigned int, unsigned int> Range;

/**
 * @class RDSNode
 * @brief Represents a node (word or pattern) in the ADIOS graph.
 *
 * Holds a lexicon unit, type, connections, and parent information.
 */
class RDSNode
{
    public:
        /**
         * @brief Lexicon unit owned by this node.
         */
        std::unique_ptr<LexiconUnit> lexicon;
        /**
         * @brief Type of the lexicon unit.
         */
        LexiconTypes::LexiconEnum type;
        /**
         * @brief Outgoing connections from this node.
         */
        std::vector<Connection> connections;
        /**
         * @brief Parent connections to this node.
         */
        std::vector<Connection> parents;

        /**
         * @brief Default constructor.
         */
        RDSNode();
        /**
         * @brief Construct from a lexicon unit and type.
         * @param lexicon Unique pointer to a lexicon unit.
         * @param type The type of the lexicon unit.
         */
        explicit RDSNode(std::unique_ptr<LexiconUnit> lexicon, LexiconTypes::LexiconEnum type);
        /**
         * @brief Copy constructor.
         * @param other The node to copy.
         */
        RDSNode(const RDSNode &other);
        /**
         * @brief Destructor.
         */
        ~RDSNode();
        /**
         * @brief Assignment operator.
         * @param other The node to assign from.
         * @return Reference to this node.
         */
        RDSNode& operator=(const RDSNode &other);
        /**
         * @brief Add a connection to this node.
         * @param con The connection to add.
         */
        void addConnection(const Connection &con);
        /**
         * @brief Get all outgoing connections.
         * @return Const reference to the vector of connections.
         */
        const std::vector<Connection>& getConnections() const;
        /**
         * @brief Set the outgoing connections.
         * @param connections The new connections vector.
         */
        void setConnections(const std::vector<Connection> &connections);
        /**
         * @brief Add a parent connection.
         * @param newParent The parent connection to add.
         * @return True if added, false if already present.
         */
        bool addParent(const Connection &newParent);
    private:
        /**
         * @brief Deep copy helper for copy constructor and assignment.
         * @param other The node to copy from.
         */
        void deepCopy(const RDSNode &other);
};

#endif
