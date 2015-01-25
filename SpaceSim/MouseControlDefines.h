#pragma once

#include <map>

namespace Input
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
            MouseWheelDown
        };

        typedef std::map<unsigned int, MouseInput> NameHashToMouseInput;
        typedef std::pair<unsigned int, MouseInput> NameHashToMouseInputPair;
        NameHashToMouseInput m_MouseToAction;
    };
}