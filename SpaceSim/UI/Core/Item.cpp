#include "UI/Core/Item.h"

#include "Core/tinyxml2.h"
#include "UI/Core/Behaviours/Behaviour.h"
#include "UI/Core/Behaviours/Registry/BehaviourRegistry.h"
#include "UI/Transistion.h"

namespace FE
{

constexpr std::string_view ItemNameTag = "name";
constexpr std::string_view ChannelName = "Item";

Item::~Item()
{
    for (auto* behaviour : m_behaviours)
    {
        delete behaviour;
    }
}

bool Item::Serialise(const tinyxml2::XMLElement* element)
{
    const tinyxml2::XMLAttribute* nameAttribute = element->FindAttribute(ItemNameTag.data());
    if (nameAttribute != nullptr)
    {
        m_id = nameAttribute->Value();
    }

    //Deserialise the behaviours here
    for (const tinyxml2::XMLElement* behaviourElement = element->FirstChildElement(); behaviourElement != nullptr; behaviourElement = behaviourElement->NextSiblingElement())
    {
        Behaviours::CreateBehaviourFP createFunction = Behaviours::GetBehaviourCreate(behaviourElement->Value());
        if (createFunction)
        {
            Behaviours::Behaviour* behaviour = createFunction();
            behaviour->Serialise(behaviourElement);
            behaviour->SetItem(this);
            m_behaviours.push_back(behaviour);
        }
        else
        {
            MSG_ERROR_CHANNEL(ChannelName.data(), "Failed to create behaviour: %s", behaviourElement->Value());
        }
    }

    return true;

}

void Item::Update(float deltaT, const InputState& input)
{
    for (auto* behaviour : m_behaviours)
    {
        behaviour->Update(deltaT, input);
    }
}

bool Item::HandleInput(const InputState& state)
{
    bool retValue = false;

    for (auto* behaviour : m_behaviours)
    {
        retValue |= behaviour->HandleInput(state);
    }

    return retValue;
}

const FE::Transition* Item::GetTransitionLink()
{
    const Transition* transition = nullptr;
    for (auto* behaviour : m_behaviours)
    {
        const auto* link = behaviour->GetTransitionLink();
        if (link != nullptr)
        {
            transition = link; //We could get more than one sadly
        }
    }

    return transition;
}

}