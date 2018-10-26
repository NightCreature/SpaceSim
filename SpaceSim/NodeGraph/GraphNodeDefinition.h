#pragma once

#include "Core/tinyxml2.h"
#include "GraphNodePin.h"

#include <vector>

namespace NodeGraph
{

class NodeDefinition
{
public:
    NodeDefinition() = default;
    ~NodeDefinition() = default;

    void Deserialise(const tinyxml2::XMLElement& nodeDefinition);
private:
    std::vector<PinDefinition> m_inputPins;
    std::vector<PinDefinition> m_outputPins;

    size_t m_nodeDefinition = 0; //links to actual node implementation
};

}