#include "Generators\astar.h"

#include <cassert>
#include <memory>



///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool SpaceSimulator::AStar::doAStar()
{
    assert(m_heuristicFunc != nullptr);
    assert(m_findNeighboursFunc != nullptr);
    assert(m_distanceFunc != nullptr);
    m_openList.push_back(&m_nodeList[m_start.Get(0) + m_dimensions.Get(0) * m_start.Get(1)]);

    while (m_openList.size() > 0)
    {
        std::sort(m_openList.begin(), m_openList.end(), [](const auto* lhs, const auto* rhs) {return lhs->m_F < rhs->m_F; });

        AStarNode* currentNode = m_openList.front();
        if (currentNode->isStart(m_end))
        {
            constructFinalPath(*currentNode);
            return true;
        }
        m_closedList.push_back(currentNode);
        if (m_openList.size() > 1)
        {
            std::swap(m_openList[0], m_openList.back());
        }
        m_openList.pop_back();
         
        std::vector<AStarNode*> neighbours = m_findNeighboursFunc(*currentNode, m_nodeList, m_dimensions.Get(0), m_dimensions.Get(1));
        for (AStarNode* node : neighbours)
        {
            auto iterator = std::find_if(m_closedList.begin(), m_closedList.end(), [node](const AStarNode* rhs) {return node->m_nodeValue.Get(0) == rhs->m_nodeValue.Get(0) && node->m_nodeValue.Get(1) == rhs->m_nodeValue.Get(1); });
            if (iterator == m_closedList.end())
            {
                float g = currentNode->m_G + m_distanceFunc(*currentNode, *node);
                node->m_G = g;
                node->m_H = m_heuristicFunc(*node, m_nodeList[m_end.Get(0) + m_dimensions.Get(0) * m_end.Get(1)]);
                node->calculateF();

                if (std::find_if(m_openList.begin(), m_openList.end(), [node](const AStarNode* rhs) {return node->m_nodeValue.Get(0) == rhs->m_nodeValue.Get(0) && node->m_nodeValue.Get(1) == rhs->m_nodeValue.Get(1); }) == m_openList.end())
                {
                    m_openList.push_back(node);
                    node->m_parent = currentNode;
                }
                else if (g < node->m_G)
                {
                    node->m_parent = currentNode;
                }
            }
        }
    }

    return false;
}
