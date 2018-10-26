#include "NodeGraph/GraphDefinitionReader.h"

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/tinyxml2.h"

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void NodeGraph::GraphReader::readGraphDefinition(const std::filesystem::path& path)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(path.string().c_str()) != tinyxml2::XML_NO_ERROR)
    {
        for (auto element = doc.FirstChildElement("NodeDefinition"); element != nullptr; element = element->NextSiblingElement())
        {
            NodeDefinition nodeDefinition;
            nodeDefinition.Deserialise(*element);
            m_nodeDefinitions.push_back(nodeDefinition);
        }
    }
}

