#pragma once

#include <map>

namespace InputDefines
{
class MouseControlDefinitions
{
public:
    MouseControlDefinitions();
    ~MouseControlDefinitions() {}
    enum class MouseInput : unsigned int
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
    MouseInput FindControllerInputIdForHashId(size_t hashId) const;
private:
    size_t m_inputNameHash[MouseInput::NumInputActions];
    MouseInput m_xControllerInputId[MouseInput::NumInputActions];
};
}