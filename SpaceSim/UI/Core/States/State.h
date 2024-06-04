#pragma once

#include "Core/StringOperations/HashString.h"

namespace tinyxml2
{

class XMLElement;

}

class InputState;
class Resource;
namespace MessageSystem { class FEMessage; }

namespace FE
{

class Transition;

namespace States
{
class State
{
public:
    State(Resource* resource) : m_resource(resource) {  }
    virtual ~State() = default;

    
    virtual bool Serialise(tinyxml2::XMLElement* element);
    virtual bool RequiresUpdate() const { return false; }

    virtual bool Activate() = 0;
    virtual void Update(float deltaTime, const InputState& input) = 0;
    virtual bool Deactivate() = 0;

    virtual bool HandleMessage(const MessageSystem::FEMessage& message) = 0;

    const Transition* GetTransition() const { return m_transition; }
    const HashString& GetName() const { return m_name; }

    static State* Create() { return nullptr; }
protected:
    Resource* m_resource = nullptr;
    bool m_receivedAnswer = false;
private:
    HashString m_name;
    Transition* m_transition = nullptr;
    
};

}

}