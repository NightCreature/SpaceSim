#include "NodeFactory.h"

#include <algorithm>


#include "Nodes/AddNode.h"

namespace NodeGraph
{

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void NodeFactory::RegisterNode(const NodeConstructionDefinition& nodeDefinition)
{
	m_nodeDefinitions.push_back(nodeDefinition);
	m_nodeDefinitionHashes.push_back(nodeDefinition.m_definitionHash);
	m_nodeHashes.push_back(nodeDefinition.m_nodeHash);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
NodeDefinition* NodeFactory::CreateDefinition(size_t nodeDefinitionHash)
{
	auto hashIt = std::find(cbegin(m_nodeDefinitionHashes), cend(m_nodeDefinitionHashes), nodeDefinitionHash);
	auto index = std::distance(cbegin(m_nodeDefinitionHashes), hashIt);
	if (hashIt != cend(m_nodeDefinitionHashes) && index > 0 && index < static_cast<ptrdiff_t>(m_nodeDefinitions.size()))
	{
		return m_nodeDefinitions[index].m_createDefinitionFP();
	}

	return nullptr;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Node* NodeFactory::CreateNode(size_t nodeHash)
{
	auto hashIt = std::find(cbegin(m_nodeHashes), cend(m_nodeHashes), nodeHash);
	auto index = std::distance(cbegin(m_nodeHashes), hashIt);
	if (hashIt != cend(m_nodeHashes) && index > 0 && index < static_cast<ptrdiff_t>(m_nodeDefinitions.size()))
	{
		return m_nodeDefinitions[index].m_createNodeFP();
	}

	return nullptr;
}

void testFactory()
{
	NodeFactory factory;
	REGISTER_CONSTRUCTION_DEFINITION(factory, NodeDefinition, AddNode)
}

}