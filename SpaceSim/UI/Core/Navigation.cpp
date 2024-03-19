#include "Navigation.h"

#include "Core/tinyxml2.h"
#include "Input/FE/NavigationActions.h"
#include "Input/InputAction.h"
#include "Input/InputState.h"
#include "Input/InputSystem.h"
#include "States/Screen.h"
#include "UI/Core/Item.h"

#include <ranges>
#include <string>

//Name is still up for change
#include "UI/Utils/DirtyValue.h"

namespace FE
{

constexpr std::string_view rowDelimiter = "\n";
//constexpr std::string_view columnDelimiters[] = { "\t", " " };

bool Navigation::Serialise(tinyxml2::XMLElement* element)
{
    //This has all kinds of weird serialisations, and needs to look at the inner text of an elelment to see whats in there
    //New lines seperate rows, spaces separate cells, if there are items with the same name in neighbouring slots it means they are the same item
    tinyxml2::XMLText* innerText = element->FirstChild()->ToText();
    if (innerText != nullptr)
    {
        std::string actualText = trim(innerText->Value());
        //Need to parse the text here for new lines and whitespace so we can do a two pass delimit which will give us the rows and then the columns from which we can extract the position information and which items are where
        auto lineTokens = tokeniseString(actualText, *(rowDelimiter.data()));
        size_t rowIndex = 0;
        m_navItems.resize(lineTokens.size()); //Allocate the rows
        for (const auto& line : lineTokens)
        {
            auto columnTokens = tokeniseString(trim(line), ' ');
            size_t columnIndex = 0;
            m_navItems[rowIndex].resize(columnTokens.size());
            for (const auto& cell : columnTokens)
            { 
                
                NavigationItem& navItem = m_navItems[rowIndex][columnIndex];
                navItem.m_position[0] = static_cast<int>(rowIndex);
                navItem.m_position[1] = static_cast<int>(columnIndex);
                navItem.m_id = HashString(cell);

                ++columnIndex;
            }
            ++rowIndex;
        }
    }

    return true;
}

bool Navigation::HandleInput(const InputState& state)
{
    //Write item navigation here
    //HasChanged<int> x = m_activeItem[0];
    //HasChanged<int> y = m_activeItem[1];
    size_t x = m_activeItem[0];
    size_t y = m_activeItem[1];
    
    if (state.getActionValue(NavigationActions::feUp) > 0.0f)
    {
        //Have to keep it in this range
        if (m_allowVerticalWrap)
        {
            //Modulo logic here we allow wrapping in this direction
            x = ( (x + 1) % m_navItems.size());
        }
        else
        {
            if (0 <= x && x < m_navItems.size() - 1)
            {
                ++x;
            }
        }
    }
    if (state.getActionValue(NavigationActions::feDown) > 0.0f)
    {
        if (m_allowVerticalWrap)
        {
            x = static_cast<int>( ((x - 1) + m_navItems.size()) % m_navItems.size()); //Keep this in positive numbers
        }
        else
        {
            if (0 < x && x < m_navItems.size() - 1)
            {
                --x;
            }
        }
    }
    if (state.getActionValue(NavigationActions::feLeft) > 0.0f)
    {
        //Have to keep it in this range
        if (m_allowHorizontalWrap)
        {
            //Modulo logic here we allow wrapping in this direction
            y = static_cast<int>((y - 1 + m_navItems[x].size()) % m_navItems[x].size());
        }
        else
        {
            if (0 < y && y < m_navItems[x].size() - 1)
            {
                --y;
            }
        }
    }
    if (state.getActionValue(NavigationActions::feRight) > 0.0f)
    {
        //Have to keep it in this range
        if (m_allowHorizontalWrap)
        {
            //Modulo logic here we allow wrapping in this direction
            y = (y + 1) % m_navItems[x].size();
        }
        else
        {
            if (0 <= y && y < m_navItems[x].size() - 1)
            {
                ++y;
            }
        }
    }

    m_activeItem[0] = x;
    m_activeItem[1] = y;

    bool retval = false;
    if (m_navItems[x][y].m_item != nullptr)
    {
        m_navItems[x][y].m_item->HandleInput(state);
    }

    return retval;
}

void Navigation::PlaceItemsInNavigation(States::Screen* screen)
{
    for (auto& row : m_navItems)
    {
        for (auto& cell : row)
        {
            cell.m_item = screen->GetItemById(cell.m_id);
        }
    }
}

}
