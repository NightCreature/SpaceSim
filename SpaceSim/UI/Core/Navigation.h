#pragma once

#include "Core/StringOperations/HashString.h"
#include "Math/VectorTypes.h"
#include "Input/InputAction.h"


namespace tinyxml2
{ 

class XMLElement;

}
class InputState;

namespace FE
{

class Item;
namespace States
{
class Screen;
}



class Navigation
{
public:
    Navigation() = default;
    bool Serialise(tinyxml2::XMLElement* element);

    bool HandleInput(const InputState& state);

    void PlaceItemsInNavigation(States::Screen* screen);
private:
    struct NavigationItem
    {
        Vector2i m_position;
        HashString m_id;
        Item* m_item = nullptr; //<- none owning pointer
    };

    std::vector < std::vector<NavigationItem>> m_navItems; //for now we only support grids, but in the future for free flow it would need to override the Serialise and storage
    Vector2i m_activeItem;
    bool m_allowHorizontalWrap = false;
    bool m_allowVerticalWrap = false;
};

}