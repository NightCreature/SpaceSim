#pragma once

#include "Core/tinyxml2.h"
#include "GraphNodePin.h"

#include <vector>

namespace NodeGraph
{

class ConnectionDefinition
{
public:
    ConnectionDefinition() = default;
    ~ConnectionDefinition() = default;

    void Deserialise(const tinyxml2::XMLElement& nodeDefinition);
private:
    size_t m_node1Id = 0;
    size_t m_node2Id = 0;
    size_t m_node1PinId = 0;
    size_t m_node2PinId = 0;
};

}