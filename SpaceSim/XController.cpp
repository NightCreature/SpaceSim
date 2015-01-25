//!#include "DXUT.h"
#include "XController.h"
#include <cmath>
#include <XInput.h>
#include <iostream>

#include "../SpaceSim/StringHelperFunctions.h"
#include "../SpaceSim/TypeHelpers.h"

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
    //if ( DPadUp & m_capabilities.Gamepad.wButtons )
    //    m_controllerState.addButtonAction(ButtonAction(ButtonAction::ePOVUp));
    //if ( DPadDown & m_capabilities.Gamepad.wButtons )
    //        m_controllerState.addButtonAction(ButtonAction(ButtonAction::ePOVDown));
    //if ( DPadLeft & m_capabilities.Gamepad.wButtons )
    //    m_controllerState.addButtonAction(ButtonAction(ButtonAction::ePOVLeft));
    //if ( DPadRight & m_capabilities.Gamepad.wButtons )
    //    m_controllerState.addButtonAction(ButtonAction(ButtonAction::ePOVRight));
    //if ( Start & m_capabilities.Gamepad.wButtons )
    //    m_controllerState.addButtonAction(ButtonAction(ButtonAction::eStartButton));
    //if ( Back & m_capabilities.Gamepad.wButtons )
    //    m_controllerState.addButtonAction(ButtonAction(ButtonAction::eBackButton));
    //if ( LeftThumb & m_capabilities.Gamepad.wButtons )
    //    m_controllerState.addButtonAction(ButtonAction(ButtonAction::eLeftThumbButton));
    //if ( RightThumb & m_capabilities.Gamepad.wButtons )
    //    m_controllerState.addButtonAction(ButtonAction(ButtonAction::eRightThumbButton));
    //if ( LB & m_capabilities.Gamepad.wButtons )
    //    m_controllerState.addButtonAction(ButtonAction(ButtonAction::eLBButton));
    //if ( RB & m_capabilities.Gamepad.wButtons )
    //    m_controllerState.addButtonAction(ButtonAction(ButtonAction::eRBButton));
    //if ( A & m_capabilities.Gamepad.wButtons )
    //    m_controllerState.addButtonAction(ButtonAction(ButtonAction::eAButton));
    //if ( B & m_capabilities.Gamepad.wButtons )
    //    m_controllerState.addButtonAction(ButtonAction(ButtonAction::eRBButton));
    //if ( X & m_capabilities.Gamepad.wButtons )
    //    m_controllerState.addButtonAction(ButtonAction(ButtonAction::eXButton));
    //if ( Y & m_capabilities.Gamepad.wButtons )
    //    m_controllerState.addButtonAction(ButtonAction(ButtonAction::eYButton));
    //
    //if (m_capabilities.Gamepad.bRightTrigger != 0 )
    //    m_controllerState.addAxisAction(AxisAction(AxisAction::eRightTrigger));
    //if (m_capabilities.Gamepad.bLeftTrigger != 0 )
    //    m_controllerState.addAxisAction(AxisAction(AxisAction::eLeftTrigger));
    //if (m_capabilities.Gamepad.sThumbLX != 0 )
    //    m_controllerState.addAxisAction(AxisAction(AxisAction::eLeftXAxis));
    //if (m_capabilities.Gamepad.sThumbLY != 0 )
    //    m_controllerState.addAxisAction(AxisAction(AxisAction::eLeftYAxis));
    //if (m_capabilities.Gamepad.sThumbRX != 0 )
    //    m_controllerState.addAxisAction(AxisAction(AxisAction::eRightXAxis));
    //if (m_capabilities.Gamepad.sThumbRY != 0 )
    //    m_controllerState.addAxisAction(AxisAction(AxisAction::eRightYAxis));
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

    //m_controllerState.setActionValue(InputActions::eAButton, m_gamepadState.wButtons & A ? true : false);
    //m_controllerState.setActionValue(InputActions::eBButton, m_gamepadState.wButtons & B ? true : false);
    //m_controllerState.setActionValue(InputActions::eXButton, m_gamepadState.wButtons & X ? true : false);
    //m_controllerState.setActionValue(InputActions::eYButton, m_gamepadState.wButtons & Y ? true : false);
    //m_controllerState.setActionValue(InputActions::eBackButton, m_gamepadState.wButtons & Back ? true : false);
    //m_controllerState.setActionValue(InputActions::eStartButton, m_gamepadState.wButtons & Start ? true : false);
    //m_controllerState.setActionValue(InputActions::eLBButton, m_gamepadState.wButtons & LB ? true : false);
    //m_controllerState.setActionValue(InputActions::eRBButton, m_gamepadState.wButtons & RB ? true : false);
    //m_controllerState.setActionValue(InputActions::ePOVUp, m_gamepadState.wButtons & DPadUp ? true : false);
    //m_controllerState.setActionValue(InputActions::ePOVDown, m_gamepadState.wButtons & DPadDown ? true : false);
    //m_controllerState.setActionValue(InputActions::ePOVLeft, m_gamepadState.wButtons & DPadLeft ? true : false);
    //m_controllerState.setActionValue(InputActions::ePOVRight, m_gamepadState.wButtons & DPadRight ? true : false);
    //m_controllerState.setActionValue(InputActions::eLeftTrigger, m_gamepadStatefloat.leftTrigger);
    //m_controllerState.setActionValue(InputActions::eRightTrigger, m_gamepadStatefloat.rightTrigger);
    //m_controllerState.setActionValue(InputActions::eLeftXAxis, m_gamepadStatefloat.leftXAxis);
    //m_controllerState.setActionValue(InputActions::eLeftYAxis, m_gamepadStatefloat.leftYAxis);
    //m_controllerState.setActionValue(InputActions::eRightXAxis, m_gamepadStatefloat.rightXAxis);
    //m_controllerState.setActionValue(InputActions::eRightYAxis, m_gamepadStatefloat.rightYAxis);

    return m_controllerState;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void XInputDevice::internalActionSetup( InputActions::ActionType inputAction, const tinyxml2::XMLAttribute* input )
{
    //Setup controller specific action map so it can map it's capabilities to the input the game expects
    UNUSEDPARAM(inputAction);
    UNUSEDPARAM(input);
}
