#pragma once

#include "IController.h"
#include "Input.h"
#include <map>
#include <string>

#include <deque>
#include <windows.h>

//-----------------------------------------------------------------------------
//! @brief   Input System
//! @remark
//-----------------------------------------------------------------------------
class InputSystem
{
public:
	InputSystem();
	~InputSystem();
    
    void initialise(const std::string& inputMapFileName);
    void update(float elapsedTime, double time);

    Input getInput() const
    {
        Input input;
        unsigned int counter = 0;
        for (std::map<IInputDevice*, InputState>::const_iterator it = m_controllers.begin(); it != m_controllers.end(); ++it)
        {
            input.setInput(counter, const_cast<InputState*>(&it->second));
            //++counter; //Uncomment this line to have multiple input sets in the input
        }

        return input;
    }

    static InputActions::ActionType getInputActionFromName(const std::string& actionName) { return getInputActionFromName(hashString(actionName.c_str())); }
    static InputActions::ActionType getInputActionFromName(unsigned int actionName);

    typedef std::map<IInputDevice*, InputState> ControllersAndState;
    typedef std::map<IInputDevice*, InputState>::iterator ControllersAndStateIt;
    typedef std::pair<IInputDevice*, InputState> ControllerAndStatePair;
    typedef std::vector<InputActions::ActionType> AvailableActions;

    static void SetRawInput(RAWINPUT* rawInput) 
    {
        m_rawInput.push_back(*rawInput); 
    }

    void addRawInputDevice(const RAWINPUTDEVICE& device) { m_devices.push_back(device); }
protected:
private:
    IInputDevice* createController(const ControllerType type);
    ControllerType stringToControllerType(const std::string& controllerName);

    ControllersAndState m_controllers;
    std::vector<RAWINPUTDEVICE> m_devices;
    static AvailableActions m_availableActions;

    static std::vector<RAWINPUT> m_rawInput;
};