#pragma once

#include "IController.h"
#include "Input.h"
#include <map>
#include <string>

#include <deque>
#include <windows.h>

///-----------------------------------------------------------------------------
///! @brief   Input System
///! @remark
///-----------------------------------------------------------------------------
class InputSystem
{
public:
	InputSystem();
	~InputSystem();
    
    void initialise(const std::string& inputMapFileName, HWND hwnd);
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

	//Expensive function
    static bool getInputActionFromName(size_t actionName, InputActions::ActionType& actionType);

    typedef std::map<IInputDevice*, InputState> ControllersAndState;
    typedef std::map<IInputDevice*, InputState>::iterator ControllersAndStateIt;
    typedef std::pair<IInputDevice*, InputState> ControllerAndStatePair;
    typedef std::vector<InputActions::ActionType> AvailableActions;

    static void SetRawInput(RAWINPUT* rawInput) 
    {
		if (RIM_TYPEKEYBOARD == rawInput->header.dwType)
		{
			m_rawKeyBoardInput.push_back(*rawInput);
		}
		else if (RIM_TYPEMOUSE == rawInput->header.dwType)
		{
			m_rawMouseInput.push_back(*rawInput);
		}
		else if (RIM_TYPEHID == rawInput->header.dwType)
		{
			m_rawHidInput.push_back(*rawInput);
		}
    }

    void addRawInputDevice(const RAWINPUTDEVICE& device) { m_devices.push_back(device); }
#ifdef DEBUG
	static std::map < InputActions::ActionType, std::string > m_actionNames;
#endif
protected:
private:
    IInputDevice* createController(const ControllerType type);
    ControllerType stringToControllerType(const std::string& controllerName);

    ControllersAndState m_controllers;
    std::vector<RAWINPUTDEVICE> m_devices;
    static AvailableActions m_availableActions;
	

	static std::vector<RAWINPUT> m_rawKeyBoardInput;
	static std::vector<RAWINPUT> m_rawMouseInput;
	static std::vector<RAWINPUT> m_rawHidInput;
};