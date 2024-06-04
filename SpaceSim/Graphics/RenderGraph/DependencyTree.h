#pragma once

#include <string>
#include <vector>
#include <dxgiformat.h>

struct DependencyNode

{
    std::vector<const DependencyNode*> m_dependsOnNodes; //Nodes that this node depends on 
};

//We build this tree from the leaves up not root down, this makes executing a graph only have to check if a node is not depending on any other node to complete. When a Node completes it can be removed from all other nodes lists
//This needs more work

class DependencyTree
{
public:
    DependencyTree() = default;
    ~DependencyTree() = default;

    void AddDependency(DependencyNode* node, const DependencyNode* nodeToDependOn);
    bool DetectCycles() const { return false; }
private:
    std::vector<const DependencyNode*> m_tree = {}; //This could be a priority queue, with the nodes without any dependencies at the front, each update will move nodes forward that have no more dependencies in the tree
};

///-----------------------------------------------------------------------------
///! @brief   
///! @remark This should probably also check to see if nodeToDependOn already is in the tree or not. If not should probably add it to the tree
///-----------------------------------------------------------------------------
void DependencyTree::AddDependency(DependencyNode* node, const DependencyNode* nodeToDependOn)
{
    if (node != nullptr)
    {
        //Should probably check that this node is not already in the list.
        std::vector<const DependencyNode*>::const_iterator dependencyIt = std::find(cbegin(node->m_dependsOnNodes), cend(node->m_dependsOnNodes), nodeToDependOn);
        if (dependencyIt == cend(node->m_dependsOnNodes))
        {
            node->m_dependsOnNodes.push_back(nodeToDependOn);
        }

        //Check to see if we need to add the node to the tree
        std::vector<const DependencyNode*>::const_iterator it = std::find(cbegin(m_tree), cend(m_tree), node);
        if (it == cend(m_tree))
        {
            m_tree.push_back(node);
        }
    }
}

struct RenderResourceProperties1
{
    std::pair<size_t, size_t> m_size;
    DXGI_FORMAT m_textureFormat = DXGI_FORMAT_R8G8B8A8_UNORM; //RT format, this does inform which pipeline objects can be used
    bool m_outResource = false; //Output RT carries this
};
struct RenderPassParameters;

template<class Function>
struct RenderPass1 : public DependencyNode
{
    std::vector<RenderResourceProperties1> m_usedResources; //This is mostly targets, and maybe already resolved pervious RTs
    std::string m_name;
    size_t m_nameHash = static_cast<size_t>(-1); //For comparison purposes, has of the m_name
    size_t m_flags; //Compute pass, graphics pass or something else
    
    virtual void Execute(const RenderPassParameters& params) = 0;
};