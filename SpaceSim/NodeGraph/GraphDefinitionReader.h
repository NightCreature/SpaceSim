#pragma once

#include "NodeGraph/GraphConnectionDefinition.h"
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
    std::vector<ConnectionDefinition> m_connectionDefinitions;
};
}