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
    Screen(Resource* resource) : State(resource) {}
    ~Screen();

    
    bool Serialise(tinyxml2::XMLElement* element) override;

    void Update(float deltaTime, const InputState& state) override;
    virtual bool Activate() override;
    virtual bool Deactivate() override;

    virtual bool HandleInput(const InputState& input);

    Item* GetItemById(const HashString& item);
    
    bool RequiresUpdate() const override { return true; }

    static State* Create(Resource* resource) { return new Screen(resource); }



    bool HandleMessage(const MessageSystem::FEMessage& message) override;
private:
    Navigation m_itemNavigation;
    std::vector<Item> m_items;

    std::string m_screenType;

    bool m_activated = false;
};
}

}