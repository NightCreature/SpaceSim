#pragma once

#include "NodeGraph/GraphNodeDefinition.h"

#include <filesystem>
#include <vector>

namespace NodeGraph
{
class GraphReader
{
public:
    void readGraphDefinition(const std::filesystem::path& path);
private:
    std::vector<NodeDefinition> m_nodeDefinitions;
};
}