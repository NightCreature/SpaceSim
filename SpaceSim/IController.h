#pragma once

#include "ControllerDefines.h"
#include "tinyxml2.h"

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
    virtual void initialise() = 0;
    virtual void deserialise(const tinyxml2::XMLElement* element, const InputSystem& inputSystem);

    virtual const InputState& update(const std::vector<RAWINPUT>& keyboardInput, const std::vector<RAWINPUT>& mouseInput, const std::vector<RAWINPUT>& hidInput) = 0;
    virtual const bool isConnected() const = 0;
    virtual void enableController() = 0;
    virtual void disableController() = 0;

    const InputState& getState() const { return m_controllerState; }
protected:
    virtual void internalActionSetup( InputActions::ActionType inputAction, const tinyxml2::XMLAttribute* input ) = 0;

    InputState m_controllerState;
    DeadZone   m_sensitivity;
};
