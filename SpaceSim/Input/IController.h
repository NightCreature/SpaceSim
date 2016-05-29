#pragma once

#include "Input/ControllerDefines.h"
#include "Core/tinyxml2.h"

class InputSystem;

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark should rename this to input Device as it is the actual hardware device in derived classes
//-----------------------------------------------------------------------------
class IInputDevice
{
public:
    IInputDevice() {}
    ~IInputDevice() {}

    //Adds actions the controller has to the inputState so we can query this object for input
    virtual void initialise(HWND hwnd) = 0;
    virtual void deserialise(const tinyxml2::XMLElement* element, const InputSystem& inputSystem);

    virtual const InputState& update(const std::vector<RAWINPUT>& keyboardInput, const std::vector<RAWINPUT>& mouseInput, const std::vector<RAWINPUT>& hidInput) = 0;
    const bool isConnected() const { return m_connected; }
    void enableController() { m_enabled = true; }
    void disableController() { m_enabled = false; }

    const InputState& getState() const { return m_controllerState; }
protected:
    virtual void internalActionSetup( InputActions::ActionType inputAction, const tinyxml2::XMLAttribute* input ) = 0;

    InputState m_controllerState;
    bool m_connected;
    bool m_enabled;
};
