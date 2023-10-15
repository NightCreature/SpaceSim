#pragma once
#include "Core/StringOperations/HashString.h"

class InputState;

namespace tinyxml2 
{
class XMLElement;
}

namespace FE
{

class Transition;

namespace Behaviours
{
class Behaviour;
}

class Item
{
public:
    ~Item();

    bool Serialise(const tinyxml2::XMLElement* element);
    void Update(float deltaT, const InputState& input);
    bool HandleInput(const InputState& state);
    const Transition* GetTransitionLink();

    void Activate();
    void Deactivate();

    const HashString& GetId() const { return m_id; }
private:
    HashString m_id;
    std::vector<Behaviours::Behaviour*> m_behaviours;
};

}