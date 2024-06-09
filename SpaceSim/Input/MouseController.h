#pragma once
#include "IController.h"
#include "Core/StringOperations/HashString.h"
#include <map>

class MouseController : public IInputDevice
{
public:
    MouseController();
    virtual ~MouseController();

    virtual void initialise(HWND hwnd) override;
    virtual const InputState& update(const std::vector<RAWINPUT>& keyboardInput, const std::vector<RAWINPUT>& mouseInput, const std::vector<RAWINPUT>& hidInput) override;

    HASH_ELEMENT_DEFINITION(MouseController)
private:
    virtual void internalActionSetup(InputActions::ActionType inputAction, const tinyxml2::XMLAttribute* input) override;

    typedef std::map<unsigned int, InputActions::ActionType> PhysicalInputMapping;
    typedef std::pair<unsigned int, InputActions::ActionType> PhysicalInputPair;
    PhysicalInputMapping m_physicalKeyToAction;
    unsigned int m_monitorWidth;
    unsigned int m_monitorHeight;
};

