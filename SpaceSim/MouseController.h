#pragma once
#include "IController.h"
#include <map>

class MouseController : public IInputDevice
{
public:
    MouseController();
    virtual ~MouseController();

    virtual void initialise() override;
    virtual const InputState& update(const std::vector<RAWINPUT>& keyboardInput, const std::vector<RAWINPUT>& mouseInput, const std::vector<RAWINPUT>& hidInput) override;

    HASH_ELEMENT_DEFINITION
private:
    virtual void internalActionSetup(InputActions::ActionType inputAction, const tinyxml2::XMLAttribute* input) override;

    typedef std::map<unsigned int, InputActions::ActionType> PhysicalInputMapping;
    typedef std::pair<unsigned int, InputActions::ActionType> PhysicalInputPair;
    PhysicalInputMapping m_physicalKeyToAction;
};

