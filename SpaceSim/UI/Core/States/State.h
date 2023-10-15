#pragma once

#include "Core/StringOperations/HashString.h"

namespace tinyxml2
{

class XMLElement;

}

class InputState;

namespace FE
{

class Transition;

namespace States
{
class State
{
public:
    State() {  }
    virtual ~State() = default;

    virtual void Update(float deltaTime, const InputState& input) = 0;
    virtual bool Serialise(tinyxml2::XMLElement* element);
    virtual bool RequiresUpdate() const { return false; }

    const Transition* GetTransition() const { return m_transition; }
    const HashString& GetName() const { return m_name; }

    static State* Create() { return nullptr; }
private:
    HashString m_name;
    Transition* m_transition;
};

}

}