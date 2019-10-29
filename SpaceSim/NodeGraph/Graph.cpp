#include "Graph.h"

#include "Core/Types/TypeHelpers.h"
#include "NodeGraph/Nodes/AddNode.h"

namespace NodeGraph
{

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
const Node& Graph::CreateNode(const NodeDefinition& node)
{
    UNUSEDPARAM(node);

    m_nodes.emplace_back(new AddNode());
    return *m_nodes[m_nodes.size() - 1];
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
GraphConnection Graph::CreateConnection(const PinBase* lhs, const PinBase* rhs)
{
    return GraphConnection(lhs, rhs);
}

}