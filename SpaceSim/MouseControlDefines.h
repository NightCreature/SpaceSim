#pragma once

#include <map>

namespace InputDefines
{
class MouseControlDefinitions
{
public:
    MouseControlDefinitions();
    ~MouseControlDefinitions() {}
    enum MouseInput
    {
        PositiveX,
        NegativeX,
        PositiveY,
        NegativeY,
        MouseButton1,
        MouseButton2,
        MouseButton3,
        MouseButton4,
        MouseButton5,
        MouseWheelUp,
        MouseWheelDown,

        NumInputActions
    };
    MouseInput FindControllerInputIdForHashId(unsigned int hashId) const;
private:
    unsigned int m_inputNameHash[NumInputActions];
    MouseInput m_xControllerInputId[NumInputActions];
};
}