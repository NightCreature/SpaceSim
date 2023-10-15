#pragma once

#include "Core/Types/TypeHelpers.h"
#include "UI/Core/Item.h"
#include "UI/Core/Navigation.h"
#include "UI/Core/States/State.h"

class InputState;

namespace FE
{

namespace States
{

class Screen : public State
{
public:
    Screen() = default;
    ~Screen();

    void Update(float deltaTime, const InputState& state) override;
    bool Serialise(tinyxml2::XMLElement* element) override;

    virtual bool HandleInput(const InputState& input);

    Item* GetItemById(const HashString& item);
    
    bool RequiresUpdate() const override { return true; }

    static State* Create() { return new Screen(); }
private:
    virtual void Activate();
    virtual void Deactivate();

    Navigation m_itemNavigation;
    std::vector<Item> m_items;
};
}

}