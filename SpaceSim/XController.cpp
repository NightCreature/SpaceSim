//!#include "DXUT.h"
#include "XController.h"
#include <cmath>
#include <XInput.h>
#include <iostream>

#include "../SpaceSim/StringHelperFunctions.h"
#include "../SpaceSim/TypeHelpers.h"
#include "../SpaceSim/XControllerDefines.h"

HASH_ELEMENT_IMPLEMENTATION(XInputDevice)

//!-----------------------------------------------------------------------------
//!! @brief Constructor
//!-----------------------------------------------------------------------------
XInputDevice::XInputDevice(const int controllerNr):
m_controllerIndex(controllerNr),
m_controllerActive(false),
m_previousPacketNr(0),
m_vibration(false),
m_isControllerActive(false),
m_firstUpdate(true)
{
    ZeroMemory(&m_gamepadState, sizeof(XINPUT_GAMEPAD));
    ZeroMemory(&m_capabilities, sizeof(XINPUT_CAPABILITIES));
    XInputGetCapabilities(m_controllerIndex, 0, &m_capabilities);
    if (m_capabilities.Type == XINPUT_DEVTYPE_GAMEPAD)
    {
        switch(m_capabilities.SubType)
        {
        case XINPUT_DEVSUBTYPE_GAMEPAD:
        default:
            m_type = Gamepad;
            break;
        }
    }
    m_connected = true;
}

//-----------------------------------------------------------------------------
//! @brief   Initialise the controller and add the capabilities to the inputState
//! @remark
//-----------------------------------------------------------------------------
void XInputDevice::initialise( )
{
}

//!-----------------------------------------------------------------------------
//!! @brief Update the gamepad state
//!! @return
//!! @remarks Need to implement setting the values in the InputState
//!-----------------------------------------------------------------------------
const InputState& XInputDevice::update(const std::vector<RAWINPUT>& keyboardInput, const std::vector<RAWINPUT>& mouseInput, const std::vector<RAWINPUT>& hidInput)
{
    UNUSEDPARAM(keyboardInput);
    UNUSEDPARAM(mouseInput);
    UNUSEDPARAM(hidInput);

    DWORD gamepadActive = 0;    
    XINPUT_STATE newState;
    ZeroMemory( &newState, sizeof(XINPUT_STATE) );

    XInputEnable(m_isControllerActive);

    gamepadActive = XInputGetState( m_controllerIndex, &newState );


    if( gamepadActive == ERROR_SUCCESS )
    { 
        if (!m_firstUpdate)
        {
            m_controllerActive = true;

            if (newState.dwPacketNumber != m_previousPacketNr)
            {
                m_previousPacketNr = newState.dwPacketNumber;
                XINPUT_GAMEPAD gamepadState = newState.Gamepad;

                float ThumbLX = gamepadState.sThumbLX / (float)(0xffff / 2);
                float ThumbLY = gamepadState.sThumbLY / (float)(0xffff / 2);
                float ThumbRX = gamepadState.sThumbRX / (float)(0xffff / 2);
                float ThumbRY = gamepadState.sThumbRY / (float)(0xffff / 2);
                float TriggerLeft = gamepadState.bLeftTrigger / (float)(0xff / 2);
                float TriggerRight = gamepadState.bRightTrigger / (float)(0xff / 2);

                //!Dead zone the controller
                if( ( ThumbLX < m_sensitivity.leftStick &&
                      ThumbLX > -m_sensitivity.leftStick ) &&
                    ( ThumbLY < m_sensitivity.leftStick &&
                      ThumbLY > -m_sensitivity.leftStick ) )
                {
                    ThumbLX = 0;
                    ThumbLY = 0;
                }
                if( ( ThumbRX < m_sensitivity.rightStick &&
                      ThumbRX > -m_sensitivity.rightStick ) &&
                    ( ThumbRY < m_sensitivity.rightStick &&
                      ThumbRY > -m_sensitivity.rightStick ) )
                {
                    ThumbRX = 0;
                    ThumbRY = 0;
                }

                //!calculate the deltas
                m_deltaState.leftXAxis = fabs(m_gamepadStatefloat.leftXAxis) - fabs(ThumbLX);
                m_deltaState.leftYAxis = fabs(m_gamepadStatefloat.leftYAxis) - fabs(ThumbLY);
                m_deltaState.rightXAxis = fabs(m_gamepadStatefloat.rightXAxis) - fabs(ThumbRX);
                m_deltaState.rightYAxis = fabs(m_gamepadStatefloat.rightYAxis) - fabs(ThumbRY);
                m_deltaState.leftTrigger = m_gamepadStatefloat.leftTrigger - TriggerLeft;
                m_deltaState.rightTrigger = m_gamepadStatefloat.rightTrigger - TriggerRight;
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
                    ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );
                    vibration.wLeftMotorSpeed = (short)(m_bigMotor * 0xffff);
                    vibration.wRightMotorSpeed = (short)(m_minorMotor * 0xffff);
                    if (XInputSetState( m_controllerIndex, &vibration ) != ERROR_SUCCESS)
                    {
                        MSG_TRACE_CHANNEL("XCONTROLLER", "Couldn't set the rumble motors" )
                    }

                }

                //DL_TRACE("New packet arrived this frame: %d", newState.dwPacketNumber);
            }
            else
            {
                //!Zero the delta the state hasn't changed in this frame
                ZeroMemory(&m_deltaState, sizeof(AnalogControls));
                //DL_TRACE("No new packet arrived this frame");
            }
        }
        else
        {
            m_firstUpdate = false;
            ZeroMemory(&m_deltaState, sizeof(AnalogControls));

            XINPUT_GAMEPAD gamepadState = newState.Gamepad;
            //!Dead zone the controller
            float ThumbLX = gamepadState.sThumbLX / (float)(0xffff / 2);
            float ThumbLY = gamepadState.sThumbLY / (float)(0xffff / 2);
            float ThumbRX = gamepadState.sThumbRX / (float)(0xffff / 2);
            float ThumbRY = gamepadState.sThumbRY / (float)(0xffff / 2);
            float TriggerLeft = gamepadState.bLeftTrigger / (float)(0xff / 2);
            float TriggerRight = gamepadState.bRightTrigger / (float)(0xff / 2);

            //!Dead zone the controller
            if( ( ThumbLX < m_sensitivity.leftStick &&
                ThumbLX > -m_sensitivity.leftStick ) &&
                ( ThumbLY < m_sensitivity.leftStick &&
                ThumbLY > -m_sensitivity.leftStick ) )
            {
                ThumbLX = 0;
                ThumbLY = 0;
            }
            if( ( ThumbRX < m_sensitivity.rightStick &&
                ThumbRX > -m_sensitivity.rightStick ) &&
                ( ThumbRY < m_sensitivity.rightStick &&
                ThumbRY > -m_sensitivity.rightStick ) )
            {
                ThumbRX = 0;
                ThumbRY = 0;
            }

            m_gamepadState = gamepadState;
            m_gamepadStatefloat.leftXAxis = ThumbLX;
            m_gamepadStatefloat.leftYAxis = ThumbLY;
            m_gamepadStatefloat.rightXAxis = ThumbRX;
            m_gamepadStatefloat.rightYAxis = ThumbRY;
            m_gamepadStatefloat.leftTrigger = TriggerLeft;
            m_gamepadStatefloat.rightTrigger = TriggerRight;
        }
    }
    else
    {
        m_controllerActive = false;
    }

    m_connected = m_controllerActive && m_isControllerActive;

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
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::XControllerDefines::LeftShoulderButton],  (float)(m_gamepadState.wButtons & LB) );
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
void XInputDevice::internalActionSetup( InputActions::ActionType inputAction, const tinyxml2::XMLAttribute* input )
{
    //Setup controller specific action map so it can map it's capabilities to the input the game expects
    Input::XControllerDefines xInputDefinitions;
    unsigned int inputHash = hashString(input->Value());
    Input::XControllerDefines::XControllerInput xControllerInput = xInputDefinitions.m_XInputToAction[inputHash];

    m_physicalKeyToAction.insert(PhysicalInputPair(xControllerInput, inputAction));
}
