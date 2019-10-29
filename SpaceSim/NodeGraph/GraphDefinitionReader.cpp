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
    if (doc.LoadFile(path.string().c_str()) == tinyxml2::XML_NO_ERROR)
    {
        for (auto element = doc.FirstChildElement()->FirstChildElement("NodeDefinition"); element != nullptr; element = element->NextSiblingElement("NodeDefinition"))
        {
            NodeDefinition nodeDefinition;
            nodeDefinition.Deserialise(*element);
            m_nodeDefinitions.push_back(nodeDefinition);
        }

        tinyxml2::XMLElement* connectionsElelment = doc.FirstChildElement()->FirstChildElement("Connections");
        if (nullptr != connectionsElelment)
        {
            for (auto element = connectionsElelment->FirstChildElement("ConnectionDefinition"); element != nullptr; element = element->NextSiblingElement("ConnectionDefinition"))
            {
                ConnectionDefinition connectionDefinition;
                connectionDefinition.Deserialise(*element);
                m_connectionDefinitions.push_back(connectionDefinition);
            }
        }
    }
}

