#include "NodeGraph/GraphNodeDefinition.h"

#include "Core/StringOperations/StringHelperFunctions.h"

namespace NodeGraph
{
    ///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void ReadPindefinitions(const tinyxml2::XMLElement* pins, std::vector<PinDefinition>& pinDefinitions)
{
    for (auto pin = pins->FirstChildElement(); pin != nullptr; pin = pin->NextSiblingElement())
    {
        PinDefinition pinDefinition;
        pinDefinition.Deserialise(*pin);
        pinDefinitions.push_back(pinDefinition);
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! <NodeDefinition nodeType = "1234567899"> <!--nodeType is a hash of the actual node type-->
///!     <InputPins>
///!         <PinDefinition ...>
///!     < / InputPins>
///!     <OutputPins>
///!         <PinDefinition ...>
///!     < / OutputPins>
///! < / NodeDefinition>
///! @remark
///-----------------------------------------------------------------------------
void NodeDefinition::Deserialise(const tinyxml2::XMLElement& nodeDefinition)
{
    for (auto attribute = nodeDefinition.FirstAttribute(); attribute; attribute = attribute->Next())
    {
        if (hashString(attribute->Name()) == "nodeType"_hash)
        {
            m_nodeDefinition = attribute->IntValue();
        }
    }

    for (auto pins = nodeDefinition.FirstChildElement(); pins != nullptr; pins = pins->NextSiblingElement())
    {
        size_t hash = hashString(pins->Name());
        if ("InputPins"_hash == hash)
        {                
            ReadPindefinitions(pins, m_inputPins);
        }
        else if ("OutputPins"_hash == hash)
        {
            ReadPindefinitions(pins, m_outputPins);
        }
    }
}

}