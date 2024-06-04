#include "GraphNode.h"

#include "Core/Types/Types.h"
#include "Math/vector2.h"
#include "Math/vector3.h"
#include "NodeGraph/GraphNodePin.h"

#include <algorithm>

namespace NodeGraph
{

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void Node::Invoke()
{

}


///-----------------------------------------------------------------------------
///! @brief This should probably be done differntly, for now this is fine
///! @remark
///-----------------------------------------------------------------------------
PinBase* CreatePin(const PinDefinition &definition)
{
    PinBase* pin = nullptr;
    switch (definition.GetType())
    {
    case PinDefinition::Type::Bool:
        pin = new Pin<bool>(definition);
        break;
    case PinDefinition::Type::Int:
        pin = new Pin<int>(definition);
        break;
    case PinDefinition::Type::Float:
        pin = new Pin<float>(definition);
        break;
    case PinDefinition::Type::Double:
        pin = new Pin<double>(definition);
        break;
    case PinDefinition::Type::Vector2:
        pin = new Pin<Vector2>(definition);
        break;
    case PinDefinition::Type::Vector3:
        pin = new Pin<Vector3>(definition);
        break;
    default:
        break;
    }

    return pin;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void Node::AddInputPin(const PinDefinition& definition)
{
    PinBase* pin = CreatePin(definition);
    m_inputPins.push_back(pin);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void Node::RemoveInputPin(const PinBase* pin)
{
    auto pinIt = std::find(m_inputPins.begin(), m_inputPins.end(), pin);
    if (pinIt != m_inputPins.end())
    {
        std::iter_swap(pinIt, m_inputPins.end());
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void Node::AddOutputPin(const PinDefinition& definition)
{

    //This is not how this should be
    PinBase* pin = CreatePin(definition);
    m_ouputPins.push_back(pin);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void Node::RemoveOutputPin(const PinBase* pin)
{
    auto pinIt = std::find(m_ouputPins.begin(), m_ouputPins.end(), pin);
    if (pinIt != m_ouputPins.end())
    {
        std::iter_swap(pinIt, m_ouputPins.end());
    }
}
}
