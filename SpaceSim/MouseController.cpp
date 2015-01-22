#include "MouseController.h"
#include "TypeHelpers.h"

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
void MouseController::initialise()
{
    RAWINPUTDEVICE rawInputDevice;

    rawInputDevice.usUsagePage = 0x01;
    rawInputDevice.usUsage = 0x02;
    rawInputDevice.dwFlags = RIDEV_NOLEGACY;   // adds HID mouse and also ignores legacy mouse messages
    rawInputDevice.hwndTarget = 0;

    //InputSystem.addRawInputDevice(rawInputDevice);

    if (RegisterRawInputDevices(&rawInputDevice, 1, sizeof(RAWINPUTDEVICE)) == FALSE)
    {
        unsigned int error = GetLastError();
        MSG_TRACE_CHANNEL("MOUSE CONTROLLER ERROR:", "Failed to register the mouse device with error: %d, %s", error, getLastErrorMessage(error));
    }
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
const InputState& MouseController::update(const std::vector<RAWINPUT>& keyboardInput, const std::vector<RAWINPUT>& mouseInput, const std::vector<RAWINPUT>& hidInput)
{
    UNUSEDPARAM(keyboardInput);
    UNUSEDPARAM(mouseInput);
    UNUSEDPARAM(hidInput);

    for (auto input : mouseInput)
    {
        //Do something with the mouse input
        RID_DEVICE_INFO deviceInfo;
        ZeroMemory(&deviceInfo, sizeof(RID_DEVICE_INFO));
        unsigned int dataPointerSize = sizeof(RID_DEVICE_INFO);
        unsigned int returnValue = GetRawInputDeviceInfo(input.header.hDevice, RIDI_DEVICEINFO, &deviceInfo, &dataPointerSize);
        if (returnValue < 0)
        {
            unsigned int error = GetLastError();
            MSG_TRACE_CHANNEL("MOUSE CONTROLLER ERROR:", "Failed to get device info for a mouse with error: %d, %s", error, getLastErrorMessage(error));
        }
        else
        {
            MSG_TRACE_CHANNEL("MOUSE CONTROLLER", "Device ID: %d", deviceInfo.mouse.dwId);
            MSG_TRACE_CHANNEL("MOUSE CONTROLLER", "Number of Buttons: %d", deviceInfo.mouse.dwNumberOfButtons);
            MSG_TRACE_CHANNEL("MOUSE CONTROLLER", "Sample Rate: %d", deviceInfo.mouse.dwSampleRate);
            MSG_TRACE_CHANNEL("MOUSE CONTROLLER", "Horizontal Wheel: %s", deviceInfo.mouse.fHasHorizontalWheel ? "true" : "false");
        }
    }

    return m_controllerState;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
const bool MouseController::isConnected() const
{
    return false;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void MouseController::enableController()
{
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void MouseController::disableController()
{
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void MouseController::internalActionSetup(InputActions::ActionType inputAction, const tinyxml2::XMLAttribute* input)
{
    UNUSEDPARAM(inputAction);
    UNUSEDPARAM(input);
}
