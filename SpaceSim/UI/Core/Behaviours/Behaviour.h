#pragma once

#include "Core/tinyxml2.h"

class InputState;

namespace FE
{

using SerialiseStream = tinyxml2::XMLNode;
class Transition;
class Item;

namespace Behaviours
{
class Behaviour
{
public:
    virtual ~Behaviour() {}

    virtual void Serialise(const tinyxml2::XMLElement* element) = 0;
    virtual void Update(float deltaT, const InputState& input) = 0;
    virtual bool HandleInput(const InputState& state) = 0;
    virtual const Transition* GetTransitionLink() = 0;

    virtual void Activate() = 0;
    virtual void Deactivate() = 0;

    void SetItem(const Item* item) { m_parent = item; }
    const Item* GetItem() const { return m_parent; }
protected:
    const Item* m_parent = nullptr;
};

}

}