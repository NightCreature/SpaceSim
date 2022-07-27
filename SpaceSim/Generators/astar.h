#pragma once

#include "Math/VectorTypes.h"

#include <deque>
#include <queue>
#include <set>
#include <vector>

namespace SpaceSimulator
{
    /// <summary>
    /// Astar map node
    /// </summary>
    /// <typeparam name="T">T needs to be newable and defines the value of this node in the node map</typeparam>
    class AStarNode //needs equals operator
    {
    public:
        AStarNode()
        {
            reset();
        }

        void reset()
        {
            m_open = false;
            m_closed = false;
            m_target = false;
            m_F = 0.0f;
            m_G = 0.0f;
            m_H = 0.0f;
            m_parent = nullptr;
            m_nodeValue = Vector2s();
        }

        bool isStart(const Vector2s& start) const
        {
            return start.equal(m_nodeValue);
        }

        void calculateF() { m_F = m_G + m_H; }

        bool  m_open = false;
        bool  m_closed = false;
        bool  m_target = false;
        float m_F = 0.0f;
        float m_G = 0.0f;
        float m_H = 0.0f;
        Vector2s m_nodeValue;
        AStarNode* m_parent = nullptr;
    };

    //input is 2D array
    typedef std::vector<AStarNode*> (*findNeighboursDelegate)(const AStarNode& current, const std::vector<AStarNode>& nodeMap, size_t maxWidth, size_t maxHeight);
    typedef float (*distanceDelegate)(const AStarNode& current, const AStarNode& node);
    typedef float (*heuristicDelegate)(const AStarNode& node, const AStarNode& end);

    /// <summary>
    /// Generic A* algorithm
    /// </summary>
    /// <typeparam name="T"></typeparam>
    class AStar
    {
    public:
        AStar(const std::vector<AStarNode>& nodeList, const Vector2s& nodeMapDimensions, Vector2s start, Vector2s end)
        {
            m_nodeList = nodeList;
            m_dimensions = nodeMapDimensions;
            m_start = start;
            m_end = end;
        }

        bool doAStar();

        //template<class T>
        void SetNeighbourFunction(SpaceSimulator::findNeighboursDelegate& func) { m_findNeighboursFunc = func; }
        template<class T>
        void SetDistanceFunction(T func) { m_distanceFunc = func; }
        template<class T>
        void SetHeuristicFunction(T func) { m_heuristicFunc = func; }

        SpaceSimulator::findNeighboursDelegate m_findNeighboursFunc = nullptr;
        SpaceSimulator::distanceDelegate m_distanceFunc = nullptr;
        SpaceSimulator::heuristicDelegate m_heuristicFunc = nullptr;

        const std::vector<AStarNode*>& Path() const { return m_path; }

    private:
        void constructFinalPath(AStarNode& currentNode)
        {
            AStarNode* temp = &currentNode;
            while (temp->m_parent != nullptr)
            {
                m_path.push_back(temp);
                temp = temp->m_parent;
            }
        }
        std::vector<AStarNode> m_nodeList;
        std::vector<AStarNode*> m_path;

        //Working vars
        std::vector<AStarNode*> m_openList; //This needs to be a priority queue, this is not good this will search the whole map instead of the fastest path
        std::vector<AStarNode*> m_closedList;

        Vector2s m_dimensions;
        Vector2s m_start;
        Vector2s m_end;


    };
}
