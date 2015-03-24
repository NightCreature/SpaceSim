#include "MouseController.h"
#include "TypeHelpers.h"
#include "MouseControlDefines.h"
#include "MathUtilityFunctions.h"

HASH_ELEMENT_IMPLEMENTATION(MouseController);

MouseController::MouseController()
{
}


MouseController::~MouseController()
{
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void MouseController::initialise(HWND hwnd)
{
    RAWINPUTDEVICE rawInputDevice;

    rawInputDevice.usUsagePage = 0x01;
    rawInputDevice.usUsage = 0x02;
    rawInputDevice.dwFlags = RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
    rawInputDevice.hwndTarget = hwnd;

    m_connected = true;
    //InputSystem.addRawInputDevice(rawInputDevice);

    if (RegisterRawInputDevices(&rawInputDevice, 1, sizeof(RAWINPUTDEVICE)) == FALSE)
    {
        m_connected = false;
        unsigned int error = GetLastError();
        MSG_TRACE_CHANNEL("MOUSE CONTROLLER ERROR:", "Failed to register the mouse device with error: %d, %s", error, getLastErrorMessage(error));
    }

    HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO info;
    info.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(monitor, &info);
    m_monitorWidth = info.rcMonitor.right - info.rcMonitor.left;
    m_monitorHeight = info.rcMonitor.bottom - info.rcMonitor.top;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
const InputState& MouseController::update(const std::vector<RAWINPUT>& keyboardInput, const std::vector<RAWINPUT>& mouseInput, const std::vector<RAWINPUT>& hidInput)
{
    UNUSEDPARAM(keyboardInput);
    UNUSEDPARAM(hidInput);

    //Postive Y means the mouse is moving down over the screen, and negative when moving up
    //Positive X means moving Right, and negative means left
    float mouseX = 0.0f;
    float mouseY = 0.0f;
    unsigned int mouseButton1 = false;
    unsigned int mouseButton2 = false;
    unsigned int mouseButton3 = false;
    unsigned int mouseButton4 = false;
    unsigned int mouseButton5 = false;
    float wheelData = 0.0f;
    //Might need to collect the mouse data over an update and then set the values
    for (auto input : mouseInput)
    {
        
        //Do something with the mouse input
        RAWMOUSE mouseState = input.data.mouse;

        //Fudge the mouse speed a bit
        mouseX += (mouseState.lLastX / (float)m_monitorWidth);
        mouseY += (mouseState.lLastY / (float)m_monitorHeight);

        mouseButton1 = RI_MOUSE_BUTTON_1_DOWN & mouseState.usButtonFlags;
        mouseButton2 = RI_MOUSE_BUTTON_2_DOWN & mouseState.usButtonFlags;
        mouseButton3 = RI_MOUSE_BUTTON_3_DOWN & mouseState.usButtonFlags;
        mouseButton4 = RI_MOUSE_BUTTON_4_DOWN & mouseState.usButtonFlags;
        mouseButton5 = RI_MOUSE_BUTTON_5_DOWN & mouseState.usButtonFlags;

        if (RI_MOUSE_WHEEL & mouseState.usButtonFlags)
        {
            short wheelDelta = (short)mouseState.usButtonData % 120;
            wheelData += wheelDelta;
        }

    }

    if (mouseX < 0)
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::NegativeX], math::clamp((float)-mouseX, 0.0f, 1.0f));
    }
    else if (mouseX > 0)
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::PositiveX], math::clamp((float)mouseX, 0.0f, 1.0f));
    }
    else
    {
        //Reset the values
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::NegativeX], 0.0f);
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::PositiveX], 0.0f);
    }

    if (mouseY < 0)
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::NegativeY], math::clamp((float)-mouseY, 0.0f, 1.0f));
    }
    else if (mouseY > 0)
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::PositiveY], math::clamp((float)mouseY, 0.0f, 1.0f));
    }
    else
    {
        //Reset the values
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::NegativeY], 0.0f);
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::PositiveY], 0.0f);
    }

    m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::MouseButton1], (float)(mouseButton1));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::MouseButton2], (float)(mouseButton2));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::MouseButton3], (float)(mouseButton3));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::MouseButton4], (float)(mouseButton4));
    m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::MouseButton5], (float)(mouseButton5));

    if (wheelData > 0)
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::MouseWheelUp], (float)(wheelData));
    }
    else
    {
        m_controllerState.setActionValue(m_physicalKeyToAction[Input::MouseControlDefinitions::MouseWheelDown], (float)(-wheelData));
    }

    return m_controllerState;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void MouseController::internalActionSetup(InputActions::ActionType inputAction, const tinyxml2::XMLAttribute* input)
{
    UNUSEDPARAM(inputAction);

    Input::MouseControlDefinitions mouseDefinitions;
    unsigned int inputHash = hashString(input->Value());
    Input::MouseControlDefinitions::MouseInput mouseInput = mouseDefinitions.m_MouseToAction[inputHash];

    m_physicalKeyToAction.insert(PhysicalInputPair(mouseInput, inputAction));
}
