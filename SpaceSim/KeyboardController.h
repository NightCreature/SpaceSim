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

    virtual void initialise(HWND hwnd) override;
    virtual const InputState& update(const std::vector<RAWINPUT>& keyboardInput, const std::vector<RAWINPUT>& mouseInput, const std::vector<RAWINPUT>& hidInput) override;

    HASH_ELEMENT_DEFINITION
private:
    RAWKEYBOARD rawInputFixup(RAWKEYBOARD input);
    void getHumanReadableChar(char*& buffer, unsigned int flags, unsigned int scanCode, bool isE0); 
    void printKeyState( PRAWINPUT prawInput );

    virtual void internalActionSetup(InputActions::ActionType inputAction, const tinyxml2::XMLAttribute* input) override;

    typedef std::map<unsigned short, InputActions::ActionType> PhysicalInputMapping;
    typedef std::pair<unsigned short, InputActions::ActionType> PhysicalInputPair;
    PhysicalInputMapping m_physicalKeyToAction;
};

 