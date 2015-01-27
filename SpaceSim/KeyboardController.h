#pragma once
#include "icontroller.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "Windows.h"
#include "WinUser.h"
#include "Windns.h"
#include "StringHelperFunctions.h"

#include <map>

class KeyboardInputDevice : public IInputDevice
{
public:
    KeyboardInputDevice(void);
    ~KeyboardInputDevice(void);

    virtual void initialise(  );
    virtual const InputState& update(const std::vector<RAWINPUT>& keyboardInput, const std::vector<RAWINPUT>& mouseInput, const std::vector<RAWINPUT>& hidInput);

    HASH_ELEMENT_DEFINITION
private:
    enum KeyState
    {
        Pressed = 0,
        Up
    };

    RAWKEYBOARD rawInputFixup(RAWKEYBOARD input);
    void getHumanReadableChar(char*& buffer, unsigned int flags, unsigned int scanCode, bool isE0); 
    void printKeyState( PRAWINPUT prawInput );
    void resetInputValues();//Should do this differently in the update loopo

    virtual void internalActionSetup( InputActions::ActionType inputAction, const tinyxml2::XMLAttribute* input );

    KeyState m_buttonState[255];
    typedef std::map<unsigned short, InputActions::ActionType> PhysicalInputMapping;
    typedef std::pair<unsigned short, InputActions::ActionType> PhysicalInputPair;
    PhysicalInputMapping m_physicalKeyToAction;
};

 