/**
 * @file ParseTree.h
 * @brief Template classes for representing and manipulating parse trees.
 *
 * Provides ParseNode and ParseTree templates for generic parse tree structures.
 */

#pragma once

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <iostream>
#include <vector>

typedef std::pair<unsigned int, unsigned int> Connection;

template <class T>
class ParseTree;

template <class T>
class ParseNode
{
    friend class ParseTree<T>;

    public:
        
        /**
        * @brief Default constructor. Initializes parent connection to (0, 0).
        */
        ParseNode()
        : the_parent(0, 0)
        {}

        /**
        * @brief Construct a node with a value and parent connection.
        * @param value The value to store in the node.
        * @param parent The parent connection (node index, child index).
        */
        ParseNode(const T &value, const Connection &parent)
        : the_value(value), the_parent(parent)
        {}

        /**
        * @brief Get the value stored in the node.
        * @return Reference to the value.
        */
        const T& value() const
        {
            return the_value;
        }

        /**
        * @brief Get the indices of this node's children.
        * @return Const reference to the vector of child indices.
        */
        const std::vector<unsigned int>& children() const
        {
            return the_children;
        }
        
        /**
        * @brief Replace a range of children with a new node, returning the subsumed children.
        * @param start The starting index of the range to replace.
        * @param finish The ending index of the range to replace.
        * @param new_node The index of the new node to insert.
        * @return Vector of indices of the subsumed children.
        */
        std::vector<unsigned int> rewireChildren(unsigned int start, unsigned int finish, unsigned new_node)
        {
            std::vector<unsigned int> subsumed_part(the_children.begin()+start, the_children.begin()+finish+1);
            the_children.erase( the_children.begin()+start, the_children.begin()+finish+1);
            the_children.insert(the_children.begin()+start, new_node);
            return subsumed_part;

        }

    private:
        T the_value;
        Connection the_parent;
        std::vector<unsigned int> the_children;
};

template <class T>
/**
 * @class ParseTree
 * @brief Generic parse tree structure.
 * @tparam T The type of value stored in each node.
 *
 * Provides methods for constructing, modifying, and printing parse trees.
 */
class ParseTree
{
    public:
        
        /**
        * @brief Default constructor. Creates a tree with a single root node.
        */
        ParseTree()
        {
            the_nodes.push_back(ParseNode<T>());   // nodes[0] is always the root
        }
    
        /**
        * @brief Construct a tree from a vector of values, each as a direct child of the root.
        * @param values The values to add as children of the root.
        */
        ParseTree(const std::vector<T> &values)
        {
            the_nodes.push_back(ParseNode<T>());   // nodes[0] is always the root
            for(unsigned int i = 0; i < values.size(); i++)
            {
                the_nodes.front().the_children.push_back(the_nodes.size());
                the_nodes.push_back(ParseNode<T>(values[i], Connection(0, i)));
            }
        }

        /**
        * @brief Get the nodes of the tree.
        * @return Const reference to the vector of nodes.
        */
        const std::vector<ParseNode<T> >& nodes() const
        {
            return the_nodes;
        }

        /**
        * @brief Replace a range of root children with a new node, making the replaced nodes children of the new node.
        * @param start The starting index of the range to replace.
        * @param finish The ending index of the range to replace.
        * @param new_node The value for the new node.
        */        
        void rewire(unsigned int start, unsigned int finish, const T &new_node)
        {
            the_nodes.push_back(ParseNode<T>(new_node, Connection(0, 0)));
            the_nodes.back().the_children = the_nodes.front().rewireChildren(start, finish, the_nodes.size()-1);
            for(unsigned int i = 0; i < the_nodes.back().the_children.size(); i++)
                the_nodes[the_nodes.back().the_children[i]].the_parent = Connection(the_nodes.size()-1, i);
        }

        /**
        * @brief Attach another parse tree as a branch at a given node.
        * @param attachPoint The index of the node to attach to.
        * @param branch The parse tree to attach.
        */
        void attach(unsigned int attachPoint, const ParseTree<T> &branch)
        {
            assert(attachPoint < the_nodes.size());

            unsigned int offset = the_nodes.size();
            for(unsigned int i = 1; i < branch.the_nodes.size(); i++)
            {
                the_nodes.push_back(branch.the_nodes[i]);
                the_nodes.back().the_parent.first = the_nodes.back().the_parent.first+offset;
                for(unsigned int j = 0; j < the_nodes.back().children().size(); j++)
                    the_nodes.back().the_children[j] = the_nodes.back().the_children[j]+offset-1;
            }

            the_nodes[offset].the_parent.first = attachPoint;
            for(unsigned int i = 0; i < branch.the_nodes[0].the_children.size(); i++)
            the_nodes[attachPoint].the_children.push_back(branch.the_nodes[0].the_children[i]+offset-1);
        }

        /**
         * @brief Print the tree structure starting from a given node.
         * @param node The index of the node to start printing from.
         * @param tab_level The indentation level.
         */
        void print(unsigned int node, unsigned int tab_level) const
        {
            for(unsigned int i = 0; i < tab_level; i++)
                std::cout << "\t";
            std::cout << node << " ---> " << the_nodes[node].the_value << std::endl;
            for(unsigned int i = 0; i < the_nodes[node].the_children.size(); i++)
                print(the_nodes[node].the_children[i], tab_level+1);
        }

    private:
        std::vector<unsigned int> the_leaves;
        std::vector<ParseNode<T> > the_nodes;
};

#endif
