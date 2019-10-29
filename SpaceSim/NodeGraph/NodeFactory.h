#pragma once

#include "NodeGraph/GraphNode.h"
#include "NodeGraph/GraphNodeDefinition.h"

#include <map>
#include <vector>

namespace NodeGraph
{

template <class T>
NodeDefinition* CreateDefinition()
{
	return new T;
}

template <class T>
Node* CreateNode()
{
	return new T;
}
typedef NodeDefinition* (*CreateNodeDefinitionFp)();
typedef Node* (*CreateNodeFp)();

struct NodeConstructionDefinition
{
	NodeConstructionDefinition(CreateNodeDefinitionFp definitionFp, CreateNodeFp nodeFP, size_t definitionHash, size_t nodeHash)
		: m_createDefinitionFP(definitionFp)
		, m_createNodeFP(nodeFP)
		, m_definitionHash(definitionHash)
		, m_nodeHash(nodeHash) 
	{}

	CreateNodeDefinitionFp m_createDefinitionFP;
	CreateNodeFp m_createNodeFP;
	size_t m_definitionHash;
	size_t m_nodeHash;	
};

#define REGISTER_CONSTRUCTION_DEFINITION(factory, definition, node) \
	factory.RegisterNode(NodeConstructionDefinition(&CreateDefinition<definition>, &CreateNode<node>, definition::m_hash, node::m_hash));

class NodeFactory
{
public:

	void RegisterNode(const NodeConstructionDefinition& nodeDefinition);

	NodeDefinition* CreateDefinition(size_t nodeDefinitionHash);
	Node* CreateNode(size_t nodeHash);
private:
	//Order in these vectors is the same, which means we can find the hashes in one of the bottom ones which will give us the index for the top one.
	std::vector<NodeConstructionDefinition> m_nodeDefinitions;
	std::vector<size_t> m_nodeDefinitionHashes;
	std::vector<size_t> m_nodeHashes;
};

}