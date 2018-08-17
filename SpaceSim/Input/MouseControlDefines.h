#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"

namespace InputDefines
{
class MouseControlDefinitions
{
public:
    MouseControlDefinitions() {}
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

    constexpr MouseControlDefinitions::MouseInput FindControllerInputIdForHashId(size_t hashId) const
    {
        for (short counter = 0; counter < static_cast<short>(MouseInput::NumInputActions); ++counter)
        {
            if (m_inputNameHash[counter] == hashId)
            {
                return static_cast<MouseInput>(counter);
            }
        }

        return MouseInput::NumInputActions;
    }

private:
    static constexpr size_t m_inputNameHash[MouseInput::NumInputActions] =
    {
        "positive_x"_hash,
        "negative_x"_hash,
        "positive_y"_hash,
        "negative_y"_hash,
        "button_1"_hash,
        "button_2"_hash,
        "button_3"_hash,
        "button_4"_hash,
        "button_5"_hash,
        "wheel_up"_hash,
        "wheel_down"_hash
    };
};
}