#pragma once

#include "InputState.h"

enum ControllerType
{
    Gamepad = 0,
    Joystick,
    Wheel,
    Keyboard,
};

//!-----------------------------------------------------------------------------
//! @brief Structure to keep track of the dead zones
//!-----------------------------------------------------------------------------
struct DeadZone
{
    float leftStick;
    float rightStick;
    float leftTrigger;
    float rightTrigger;
};

struct ControllerCheck  
{
    bool m_xInputDeviceFound;
    ControllerType m_controllerType;
};