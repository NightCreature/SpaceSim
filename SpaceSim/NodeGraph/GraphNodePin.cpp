#include "GraphNodePin.h"

#include "Core/StringOperations/StringHelperFunctions.h"

///-----------------------------------------------------------------------------
///! @brief eg: <PinDefinition type="1" isInPin="false"/>
///! @remark for now this is xml
///-----------------------------------------------------------------------------
void NodeGraph::PinDefinition::Deserialise(const tinyxml2::XMLElement& pinDefinition)
{
    for (auto attribute = pinDefinition.FirstAttribute(); attribute != nullptr; attribute = attribute->Next())
    {
        switch (hashString(attribute->Name()))
        {
        case "type"_hash:
        {
            auto typeValue = attribute->IntValue();
            if (0 < typeValue && typeValue < static_cast<int>(Type::Count))
            {
                m_type = static_cast<Type>(typeValue);
            }
        }
        break;
        case "isInPin"_hash:
        {
            m_isInPin = attribute->BoolValue();
        }
        break;
        case "id"_hash:
        {
            m_id = attribute->IntValue();
        }
        break;
        }
    }
}
