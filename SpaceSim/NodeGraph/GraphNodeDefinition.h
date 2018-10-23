#pragma once

#include "GraphNodePin.h"

#include <vector>

namespace NodeGraph
{

class NodeDefinition
{
public:

private:
    std::vector<PinDefinition> m_inputPins;
    std::vector<PinDefinition> m_outputPins;

    size_t m_nodeDefinition; //links to actual node implementation
}

}