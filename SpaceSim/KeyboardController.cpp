#include "KeyboardController.h"
#include "StringHelperFunctions.h"
#include "TypeHelpers.h"

#include <limits>

HASH_ELEMENT_IMPLEMENTATION(KeyboardInputDevice)

KeyboardInputDevice::KeyboardInputDevice(void)
{
}


KeyboardInputDevice::~KeyboardInputDevice(void)
{
}

void KeyboardInputDevice::initialise()
{
    RAWINPUTDEVICE rawInputDevice;
    rawInputDevice.usUsagePage = 1;
    rawInputDevice.usUsage = 6;
    rawInputDevice.dwFlags = 0;
    rawInputDevice.hwndTarget = 0;

    if (RegisterRawInputDevices(&rawInputDevice, 1, sizeof(RAWINPUTDEVICE)) == FALSE)
    {
        MSG_TRACE_CHANNEL("KEYBOARD CONTROLLER ERROR:", "Failed to register the keyboard device with error: %d", GetLastError());
    }
}

const InputState& KeyboardInputDevice::update()
{
    unsigned int inputStructureSize = 0;
    //RAWINPUT rawInput;
    if (GetRawInputBuffer(0, &inputStructureSize, sizeof(RAWINPUTHEADER)) != 0 )
    {
        MSG_TRACE_CHANNEL("KEYBOARD CONTROLLER ERROR:", "Failed to grab raw input header!");
        return m_controllerState;
    }
    inputStructureSize *= 16;
    PRAWINPUT rawInput = (PRAWINPUT)malloc(inputStructureSize);
    if (rawInput == 0)
    {
        return m_controllerState;
    }

    unsigned int sizeT = inputStructureSize;
    unsigned int input = GetRawInputBuffer(rawInput, &sizeT, sizeof(RAWINPUTHEADER));

    if (input == 0)
    {
        free(rawInput);
        //resetInputValues();
        return m_controllerState;
    }
    if (input == (unsigned int)(-1))
    {
        MSG_TRACE_CHANNEL("KEYBOARD CONTROLLER ERROR:", "Failed to grab the input buffer with error: 0x%x", GetLastError());
        free(rawInput);
        //resetInputValues();
        return m_controllerState;
    }
    
    PRAWINPUT prawInput = rawInput;
    for(size_t counter = 0; counter < input; ++counter)
    {
        if (prawInput->header.dwType == RIM_TYPEKEYBOARD)
        {
            prawInput->data.keyboard = rawInputFixup(prawInput->data.keyboard);
            //printKeyState(prawInput);
            PhysicalInputMapping::const_iterator it = m_physicalKeyToAction.find(prawInput->data.keyboard.VKey);
            if (it != m_physicalKeyToAction.end())
            {
                m_controllerState.setActionValue(it->second,  (float)(((KeyState)RI_KEY_MAKE & prawInput->data.keyboard.Flags)) == Pressed);
                m_buttonState[prawInput->data.keyboard.VKey] = (KeyState)(RI_KEY_MAKE & prawInput->data.keyboard.Flags);
            }
        }

        DefRawInputProc(&prawInput, input, sizeof(RAWINPUTHEADER));
        prawInput = NEXTRAWINPUTBLOCK(prawInput);
    }

    free(rawInput);
    return m_controllerState;
}

void KeyboardInputDevice::enableController()
{
}

void KeyboardInputDevice::disableController()
{
}

//Fix code for raw input.
RAWKEYBOARD KeyboardInputDevice::rawInputFixup(RAWKEYBOARD input)
{
    unsigned int virtualKey = input.VKey;
    unsigned int scanCode = input.MakeCode;
    unsigned int flags = input.Flags;
    
    if (virtualKey == 255)
    {
        // discard "fake keys" which are part of an escaped sequence
        return RAWKEYBOARD();
    }
    else if (virtualKey == VK_SHIFT)
    {
        // correct left-hand / right-hand SHIFT
        virtualKey = MapVirtualKey(scanCode, MAPVK_VSC_TO_VK_EX);
    }
    else if (virtualKey == VK_NUMLOCK)
    {
        // correct PAUSE/BREAK and NUM LOCK silliness, and set the extended bit
        scanCode = (MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC) | 0x100);
    }
    // e0 and e1 are escape sequences used for certain special keys, such as PRINT and PAUSE/BREAK.
    // see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
    const bool isE0 = ((flags & RI_KEY_E0) != 0);
    const bool isE1 = ((flags & RI_KEY_E1) != 0);
    
    if (isE1)
    {
        // for escaped sequences, turn the virtual key into the correct scan code using MapVirtualKey.
        // however, MapVirtualKey is unable to map VK_PAUSE (this is a known bug), hence we map that by hand.
        if (virtualKey == VK_PAUSE)
            scanCode = 0x45;
        else
            scanCode = MapVirtualKey(virtualKey, MAPVK_VK_TO_VSC);
    }
    
    switch (virtualKey)
    {
        // right-hand CONTROL and ALT have their e0 bit set
    case VK_CONTROL:
        if (isE0)
            virtualKey = VK_RCONTROL; //Should be flagged as right control 
        else
            virtualKey = VK_LCONTROL; //Should be flagged as left control 
        break;
    
    case VK_MENU:
        if (isE0)
            virtualKey = VK_RMENU;//Should be flagged differently, right alt
        else
            virtualKey =VK_LMENU;//Should be flagged differently, left alt
        break;
    
        // NUMPAD ENTER has its e0 bit set
    case VK_RETURN:
        if (isE0)
            virtualKey = VK_RETURN; //Should be flagged differently
        break;
    
        // the standard INSERT, DELETE, HOME, END, PRIOR and NEXT keys will always have their e0 bit set, but the
        // corresponding keys on the NUMPAD will not.
    case VK_INSERT:
        if (!isE0)
            virtualKey = VK_NUMPAD0;
        break;
    
    case VK_DELETE:
        if (!isE0)
            virtualKey = VK_DECIMAL;
        break;
    
    case VK_HOME:
        if (!isE0)
            virtualKey = VK_NUMPAD7;
        break;
    
    case VK_END:
        if (!isE0)
            virtualKey = VK_NUMPAD1;
        break;
    
    case VK_PRIOR:
        if (!isE0)
            virtualKey = VK_NUMPAD9;
        break;
    
    case VK_NEXT:
        if (!isE0)
            virtualKey = VK_NUMPAD3;
        break;
    
        // the standard arrow keys will always have their e0 bit set, but the
        // corresponding keys on the NUMPAD will not.
    case VK_LEFT:
        if (!isE0)
            virtualKey = VK_NUMPAD4;
        break;
    
    case VK_RIGHT:
        if (!isE0)
            virtualKey = VK_NUMPAD6;
        break;
    
    case VK_UP:
        if (!isE0)
            virtualKey = VK_NUMPAD8;
        break;
    
    case VK_DOWN:
        if (!isE0)
            virtualKey = VK_NUMPAD2;
        break;
    
        // NUMPAD 5 doesn't have its e0 bit set
    case VK_CLEAR:
        if (!isE0)
            virtualKey = VK_NUMPAD5;
        break;
    }

    RAWKEYBOARD output;
    output = input;
    output.VKey = (unsigned short)virtualKey;
    output.MakeCode = (unsigned short)scanCode;
    return output;
}

void KeyboardInputDevice::getHumanReadableChar(char*& buffer, unsigned int flags, unsigned int scanCode, bool isE0)
{
    // a key can either produce a "make" or "break" scancode. this is used to differentiate between down-presses and releases
    // see http://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
    //const bool wasUp = ((flags & RI_KEY_BREAK) != 0);
    UNUSEDPARAM(flags);

    // getting a human-readable string
    unsigned int key = (scanCode << 16) | (isE0 << 24);
    buffer = new char[512];
    GetKeyNameText((long)key, buffer, 512);
}

void KeyboardInputDevice::printKeyState( PRAWINPUT prawInput )
{
    MSG_TRACE("VKEY code pressed %d", prawInput->data.keyboard.VKey);
    MSG_TRACE("SCAN code pressed %d", prawInput->data.keyboard.MakeCode);
    MSG_TRACE("Flags pressed %d", prawInput->data.keyboard.Flags);
    char* buffer;
    getHumanReadableChar(buffer, prawInput->data.keyboard.Flags, prawInput->data.keyboard.MakeCode, ((prawInput->data.keyboard.Flags & RI_KEY_E1) != 0));
    MSG_TRACE("Character pressed %s", buffer);
    delete buffer;
}

void KeyboardInputDevice::resetInputValues()
{
    memset(&m_buttonState, 1, sizeof(KeyState) * 255);
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void KeyboardInputDevice::internalActionSetup( InputActions::ActionType inputAction, const tinyxml2::XMLAttribute* input )
{
    //Setup controller specific action map so it can map it's capabilities to the input the game expects
    //XML value has to be unsigned key code
    unsigned int vk_key = input->UnsignedValue();
    m_physicalKeyToAction.insert(PhysicalInputPair((unsigned short)vk_key, inputAction));
    
}
