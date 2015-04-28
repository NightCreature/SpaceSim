//!#include "DXUT.h"
#include "XController.h"
#include <cmath>
#include <XInput.h>
#include <iostream>

#include "../SpaceSim/StringHelperFunctions.h"
#include "../SpaceSim/TypeHelpers.h"
#include "../SpaceSim/XControllerDefines.h"
#include "vector4.h"

HASH_ELEMENT_IMPLEMENTATION(XInputDevice)

//!-----------------------------------------------------------------------------
//!! @brief Constructor
//!-----------------------------------------------------------------------------
XInputDevice::XInputDevice(const int controllerNr) :
m_controllerIndex(controllerNr),
m_controllerActive(false),
m_previousPacketNr(0),
m_vibration(false)
{
    ZeroMemory(&m_gamepadState, sizeof(XINPUT_GAMEPAD));
    ZeroMemory(&m_capabilities, sizeof(XINPUT_CAPABILITIES));
    if (XInputGetCapabilities(m_controllerIndex, 0, &m_capabilities) != ERROR_DEVICE_NOT_CONNECTED)
    {
    if (m_capabilities.Type == XINPUT_DEVTYPE_GAMEPAD)
    {
        switch (m_capabilities.SubType)
        {
        case XINPUT_DEVSUBTYPE_GAMEPAD:
        default:
            m_type = Gamepad;
            break;
        }
    }
    m_connected = true;
    }
    else
    {
        m_connected = false;
        m_controllerActive = false;
        m_enabled = false;
    }
}

//-----------------------------------------------------------------------------
//! @brief   Initialise the controller and add the capabilities to the inputState
//! @remark
//-----------------------------------------------------------------------------
void XInputDevice::initialise(HWND hwnd)
{
    UNUSEDPARAM(hwnd);
}

//!-----------------------------------------------------------------------------
//!! @brief Update the gamepad state
//!! @return
//!! @remarks Need to implement setting the values in the InputState
// Need to actually only call this when a device is attached seems to be expensive when device is not there 6.9% of cpu trace
//!-----------------------------------------------------------------------------
const InputState& XInputDevice::update(const std::vector<RAWINPUT>& keyboardInput, const std::vector<RAWINPUT>& mouseInput, const std::vector<RAWINPUT>& hidInput)
{
    if (!m_enabled)
    {
        return m_controllerState;
    }
    UNUSEDPARAM(keyboardInput);
    UNUSEDPARAM(mouseInput);
    UNUSEDPARAM(hidInput);

    DWORD gamepadActive = 0;
    XINPUT_STATE newState;
    ZeroMemory(&newState, sizeof(XINPUT_STATE));

    XInputEnable(m_enabled);

    gamepadActive = XInputGetState(m_controllerIndex, &newState);


    if (gamepadActive == ERROR_SUCCESS)
    {
        m_controllerActive = true;

        if (newState.dwPacketNumber != m_previousPacketNr)
        {
            m_previousPacketNr = newState.dwPacketNumber;
            XINPUT_GAMEPAD gamepadState = newState.Gamepad;

            //determine how far the controller is pushed
            Vector4 leftStickPositionAndMagnitude;
            Vector4 rightStickPositionAndMagnitude;
            calculateThumbStickDirectionAndMagnitude(gamepadState.sThumbLX, gamepadState.sThumbLY, true, leftStickPositionAndMagnitude);
            calculateThumbStickDirectionAndMagnitude(gamepadState.sThumbRX, gamepadState.sThumbRY, false, rightStickPositionAndMagnitude);


            float ThumbLX = leftStickPositionAndMagnitude.x() * leftStickPositionAndMagnitude.w();
            float ThumbLY = leftStickPositionAndMagnitude.y() * leftStickPositionAndMagnitude.w();
            float ThumbRX = rightStickPositionAndMagnitude.x() * rightStickPositionAndMagnitude.w();
            float ThumbRY = rightStickPositionAndMagnitude.y() * rightStickPositionAndMagnitude.w();
            float TriggerLeft = gamepadState.bLeftTrigger / (float)(0xff / 2);
            float TriggerRight = gamepadState.bRightTrigger / (float)(0xff / 2);

            //!Replace previous state with current state for none delta enabled getting of stuff
            m_gamepadState = gamepadState;
            m_gamepadStatefloat.leftXAxis = ThumbLX;
            m_gamepadStatefloat.leftYAxis = ThumbLY;
            m_gamepadStatefloat.rightXAxis = ThumbRX;
            m_gamepadStatefloat.rightYAxis = ThumbRY;
            m_gamepadStatefloat.leftTrigger = TriggerLeft;
            m_gamepadStatefloat.rightTrigger = TriggerRight;

            if (m_vibration)
            {
                XINPUT_VIBRATION vibration;
                ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
                vibration.wLeftMotorSpeed = (short)(m_bigMotor * 0xffff);
                vibration.wRightMotorSpeed = (short)(m_minorMotor * 0xffff);
                if (XInputSetState(m_controllerIndex, &vibration) != ERROR_SUCCESS)
                {
                    MSG_TRACE_CHANNEL("XCONTROLLER", "Couldn't set the rumble motors")
                }

            }

            //DL_TRACE("New packet arrived this frame: %d", newState.dwPacketNumber);
        }
    }
    else
    {
        m_controllerActive = false;
    }

    m_connected = m_controllerActive && m_enabled;

    if (m_gamepadStatefloat.leftXAxis > 0.f)
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::LeftStickPositiveX], m_gamepadStatefloat.leftXAxis);
    }
    else if (m_gamepadStatefloat.leftXAxis < 0.f)
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::LeftStickNegativeX], -m_gamepadStatefloat.leftXAxis);
    }
    else
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::LeftStickPositiveX], 0.0f);
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::LeftStickNegativeX], 0.0f);
    }

    if (m_gamepadStatefloat.leftYAxis > 0.f)
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::LeftStickPositiveY], m_gamepadStatefloat.leftYAxis);
    }
    else if (m_gamepadStatefloat.leftYAxis < 0.f)
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::LeftStickNegativeY], -m_gamepadStatefloat.leftYAxis);
    }
    else
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::LeftStickPositiveY], 0.0f);
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::LeftStickNegativeY], 0.0f);
    }

    if (m_gamepadStatefloat.rightXAxis > 0.f)
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::RightStickPositiveX], m_gamepadStatefloat.rightXAxis);
    }
    else if (m_gamepadStatefloat.rightXAxis < 0.f)
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::RightStickNegativeX], -m_gamepadStatefloat.rightXAxis);
    }
    else
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::RightStickPositiveX], 0.0f);
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::RightStickNegativeX], 0.0f);
    }

    if (m_gamepadStatefloat.rightYAxis > 0.f)
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::RightStickPositiveY], m_gamepadStatefloat.rightYAxis);
    }
    else if (m_gamepadStatefloat.rightYAxis < 0.f)
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::RightStickNegativeY], -m_gamepadStatefloat.rightYAxis);
    }
    else
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::RightStickPositiveY], 0.0f);
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::RightStickNegativeY], 0.0f);
    }
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::LeftTrigger], m_gamepadStatefloat.leftTrigger);
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::RightTrigger], m_gamepadStatefloat.rightTrigger);
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::LeftShoulderButton], (float)(m_gamepadState.wButtons & LB));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::RightShoulderButton], (float)(m_gamepadState.wButtons & RB));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::BackButton], (float)(m_gamepadState.wButtons & Back));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::StartButton], (float)(m_gamepadState.wButtons & Start));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::AButton], (float)(m_gamepadState.wButtons & A));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::BBUtton], (float)(m_gamepadState.wButtons & B));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::XButton], (float)(m_gamepadState.wButtons & X));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::YButton], (float)(m_gamepadState.wButtons & Y));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::POVLeft], (float)(m_gamepadState.wButtons & DPadLeft));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::POVRight], (float)(m_gamepadState.wButtons & DPadRight));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::POVUp], (float)(m_gamepadState.wButtons & DPadUp));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::POVDown], (float)(m_gamepadState.wButtons & DPadDown));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::LeftStickClick], (float)(m_gamepadState.wButtons & LeftThumb));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::RightStickClick], (float)(m_gamepadState.wButtons & RightThumb));

    return m_controllerState;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void XInputDevice::internalActionSetup(InputActions::ActionType inputAction, const tinyxml2::XMLAttribute* input)
{
    //Setup controller specific action map so it can map it's capabilities to the input the game expects
    Input::XControllerDefines xInputDefinitions;
    unsigned int inputHash = hashString(input->Value());
    Input::XControllerDefines::XControllerInput xControllerInput = xInputDefinitions.m_XInputToAction[inputHash];

    m_physicalKeyToAction.insert(PhysicalInputPair(xControllerInput, inputAction));
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void XInputDevice::calculateThumbStickDirectionAndMagnitude(float stickX, float stickY, bool isLeftStick, Vector4& directionAndMagnitude)
{
    float magnitude = sqrt(stickX*stickX + stickY*stickY);

    //determine the direction the controller is pushed
    float normalizedLX = stickX / magnitude;
    float normalizedLY = stickY / magnitude;
    UNUSEDPARAM(normalizedLX);
    UNUSEDPARAM(normalizedLY);
    float normalizedMagnitude = 0;

    //check if the controller is outside a circular dead zone
    float deadZoneValue = isLeftStick == true ? (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE : (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
    if (magnitude > deadZoneValue)
    {
        //clip the magnitude at its expected maximum value
        if (magnitude > 32767) magnitude = 32767;

        //adjust magnitude relative to the end of the dead zone
        magnitude -= deadZoneValue;

        //optionally normalize the magnitude with respect to its expected range
        //giving a magnitude value of 0.0 to 1.0
        normalizedMagnitude = magnitude / (32767 - deadZoneValue);
    }
    else //if the controller is in the deadzone zero out the magnitude
    {
        magnitude = 0.0;
        normalizedMagnitude = 0.0;
    }

    directionAndMagnitude = Vector4(normalizedLX, normalizedLY, magnitude, normalizedMagnitude);
}
