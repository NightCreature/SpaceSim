#include "Screen.h"

#include "Core/MessageSystem/MessageQueue.h"
#include "Core/Resource/GameResource.h"

#include "UI/Core/Item.h"
#include "UI/Messages.h"

#include <algorithm>




namespace FE
{

namespace States
{

constexpr std::string_view navigationXMLTag = "navigation";
//constexpr std::string_view screenTypeTag = "screen_type";

constexpr size_t debugScreenType = "Generic"_hash;

bool Screen::HandleInput(const InputState& state)
{
    UNUSEDPARAM(state);
    return false;
}

FE::Item* Screen::GetItemById(const HashString& item)
{
    Item* screenItem = nullptr;

    std::vector<Item>::iterator iter = std::find_if(std::begin(m_items), std::end(m_items), [item](const Item& itemIterator) { return itemIterator.GetId() == item; });
    if (std::end(m_items) != iter)
    {
        screenItem = &(*iter);
    }

    return screenItem;
}

bool Screen::HandleMessage(const MessageSystem::FEMessage& message)
{
    //This expects a message that a render object has been created back
    UNUSEDPARAM(message);
    return true;
}

/// <summary>
/// Deals with stuff that happens on screen activation
/// </summary>
bool Screen::Activate()
{
    if (!m_receivedAnswer)
    {
        //Have to send a message to create a render object
        MessageSystem::FE::CreateScreen createScreen;
        createScreen.SetObject(m_screenType.empty() ? HashString(debugScreenType) : HashString(m_screenType));
        m_resource->m_messageQueues->getUpdateMessageQueue()->addMessage(createScreen);
        return false;
    }

    return true;
}

/// Deals with stuff that happens on screen deactivation
bool Screen::Deactivate()
{
    return true;
}

Screen::~Screen()
{
    m_items.clear();
}

void Screen::Update(float deltaTime, const InputState& input)
{
    UNUSEDPARAM(deltaTime);
    UNUSEDPARAM(input);

    m_itemNavigation.HandleInput(input);
}

bool Screen::Serialise(tinyxml2::XMLElement* element)
{
    State::Serialise(element);

    for (const tinyxml2::XMLAttribute* attribute = element->FirstAttribute(); attribute != nullptr; attribute = attribute->Next())
    {
        auto attributeHash = HashString(attribute->Name());
        switch (attributeHash)
        {
        case debugScreenType:
            m_screenType = attribute->Value();
            break;
        default:
            break;
        }
    }

    tinyxml2::XMLElement* navigationElement = element->FirstChildElement(navigationXMLTag.data());

    if (navigationElement != nullptr)
    {
        m_itemNavigation.Serialise(navigationElement);


        for (tinyxml2::XMLElement* childElement = navigationElement->NextSiblingElement(); childElement != nullptr; childElement = childElement->NextSiblingElement())
        {
            m_items.emplace_back();
            Item& item = m_items.back();
            item.Serialise(childElement);
        }

        m_itemNavigation.PlaceItemsInNavigation(this);
    }

    return true;
}

}

}