#include "Screen.h"

#include "UI/Core/Item.h"

#include <algorithm>

namespace FE
{

namespace States
{

constexpr std::string_view navigationXMLTag = "navigation";

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

/// <summary>
/// Deals with stuff that happens on screen activation
/// </summary>
void Screen::Activate()
{

}

/// Deals with stuff that happens on screen deactivation
void Screen::Deactivate()
{

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

    tinyxml2::XMLElement* navigationElement = element->FirstChildElement(navigationXMLTag.data());
    if (navigationElement != nullptr)
    {
        m_itemNavigation.Serialise(navigationElement);
    }

    for (tinyxml2::XMLElement* childElement = navigationElement->NextSiblingElement(); childElement != nullptr; childElement = childElement->NextSiblingElement())
    {
        m_items.emplace_back();
        Item& item = m_items.back();
        item.Serialise(childElement);
    }

    m_itemNavigation.PlaceItemsInNavigation(this);

    return true;
}

}

}