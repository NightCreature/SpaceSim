#include "NodeGraph/GraphConnectionDefinition.h"

#include "Core/StringOperations/StringHelperFunctions.h"

namespace NodeGraph
{

///-----------------------------------------------------------------------------
///! @brief 
///!  <ConnectionDefinition node1Id="1" node2Id="2" node1PinId="5" node2PinId="1" />
///! @remark
///-----------------------------------------------------------------------------
void ConnectionDefinition::Deserialise(const tinyxml2::XMLElement& nodeDefinition)
{
    for (auto attribute = nodeDefinition.FirstAttribute(); attribute; attribute = attribute->Next())
    {
        switch (hashString(attribute->Name()))
        {
        case "node1Id"_hash:
        {
            m_node1Id = attribute->IntValue();
        }
        break;
        case "node2Id"_hash:
        {
            m_node2Id = attribute->IntValue();
        }
        break;
        case "node1PinId"_hash:
        {
            m_node1PinId = attribute->IntValue();
        }
        break;
        case "node2PinId"_hash:
        {
            m_node2PinId = attribute->IntValue();
        }
        break;
        default:
            break;
        }
    }
}

}