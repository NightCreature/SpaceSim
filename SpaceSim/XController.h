#pragma once

#include "IController.h"
#include <Windows.h>
#include <XInput.h>
#include "StringHelperFunctions.h"

//!-----------------------------------------------------------------------------
//! @brief Structure to keep track of the analog deltas
//!-----------------------------------------------------------------------------
struct AnalogControls
{
    float leftXAxis;
    float leftYAxis;
    float rightXAxis;
    float rightYAxis;
    float leftTrigger;
    float rightTrigger;
};

enum ControllerActions
{
    DPadUp =     0x00000001,
    DPadDown =   0x00000002,
    DPadLeft =   0x00000004,
    DPadRight =  0x00000008,
    Start =      0x00000010,
    Back =       0x00000020,
    LeftThumb =  0x00000040,
    RightThumb = 0x00000080,
    LB =         0x00000100,
    RB =         0x00000200,
    A =          0x00001000,
    B =          0x00002000,
    X =          0x00004000,
    Y =          0x00008000,
    //!last for are options to get if left or right is pressed on either the dpad or the left stick
    LLeft =       0x00100000,
    LRight =      0x00200000,
    LUp =         0x00400000,
    LDown =       0x00800000,
    RLeft =       0x01000000,
    RRight =      0x02000000,
    RUp =         0x04000000,
    RDown =       0x08000000,
};

//!-----------------------------------------------------------------------------
//! @brief XBox controller pad
//!-----------------------------------------------------------------------------
class XInputDevice : public IInputDevice
{
public:
	XInputDevice(const int controllerNr);
    ~XInputDevice() {}

    void initialise();

    const InputState& update(const std::vector<RAWINPUT>& keyboardInput, const std::vector<RAWINPUT>& mouseInput, const std::vector<RAWINPUT>& hidInput);

    const bool isConnected() const { return m_controllerActive && m_isControllerActive; }

    void enableVibration() { m_vibration = true; }
    void disableVibration() { m_vibration = false; }
    void setMinorMotor(const float value) { m_minorMotor = value; }
    void setBigMotor(const float value) { m_bigMotor = value; }

    void enableController()  { m_isControllerActive = true; }
    void disableController() { m_isControllerActive = false; }

    HASH_ELEMENT_DEFINITION;
protected:
private:
    virtual void internalActionSetup( InputActions::ActionType , const tinyxml2::XMLAttribute* input );

    int            m_controllerIndex;
    bool           m_controllerActive;
    XINPUT_GAMEPAD m_gamepadState;
    XINPUT_CAPABILITIES m_capabilities;
    AnalogControls m_gamepadStatefloat;
    AnalogControls m_deltaState;
    DWORD          m_previousPacketNr;
    float          m_bigMotor;
    float          m_minorMotor;
    bool           m_vibration;
    bool           m_isControllerActive;
    bool           m_firstUpdate;
    ControllerType m_type;
};
