#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"
#include "NodeGraph/GraphNode.h"

#include <vector>


namespace NodeGraph
{

class NodeDefinition;

class Graph
{
public:
    Graph();
    ~Graph();

    const Node& CreateNode(const NodeDefinition& node);
    GraphConnection CreateConnection(const PinBase* lhs, const PinBase* rhs);

    HASH_ELEMENT_DEFINITION(Graph);
private:
    std::vector<Node*> m_nodes;
};

}