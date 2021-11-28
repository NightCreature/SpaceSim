#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"
#include <vector>

namespace InputDefines
{

class XControllerDefines
{
public:
    XControllerDefines() {}
    ~XControllerDefines() {}

    enum class XControllerInput : size_t
    {
        LeftStickPositiveX,
        LeftStickNegativeX,
        LeftStickPositiveY,
        LeftStickNegativeY,
        RightStickPositiveX,
        RightStickNegativeX,
        RightStickPositiveY,
        RightStickNegativeY,
        LeftTrigger,
        RightTrigger,
        LeftShoulderButton,
        RightShoulderButton,
        BackButton,
        StartButton,
        AButton,
        BBUtton,
        XButton,
        YButton,
        POVLeft,
        POVRight,
        POVUp,
        POVDown,
        LeftStickClick,
        RightStickClick,

        NumInputActions
    };

    
    constexpr XControllerInput FindControllerInputIdForHashId(size_t hashId) const
    {
        for (short counter = 0; counter < static_cast<short>(XControllerInput::NumInputActions); ++counter)
        {
            if (m_inputNameHash[counter] == hashId)
            {
                return static_cast<XControllerInput>(counter);
            }
        }

        return XControllerInput::NumInputActions;
    }
private:
    static constexpr size_t m_inputNameHash[static_cast<std::underlying_type_t<XControllerInput>>(XControllerInput::NumInputActions)] =
    {
        "left_stick_positive_x"_hash,
        "left_stick_negative_x"_hash,
        "left_stick_positive_y"_hash,
        "left_stick_negative_y"_hash,
        "right_stick_positive_x"_hash,
        "right_stick_negative_x"_hash,
        "right_stick_positive_y"_hash,
        "right_stick_negative_y"_hash,
        "left_trigger"_hash,
        "right_trigger"_hash,
        "left_shoulder_button"_hash,
        "right_shoulder_button"_hash,
        "back_button"_hash,
        "start_button"_hash,
        "a_button"_hash,
        "b_button"_hash,
        "x_button"_hash,
        "y_button"_hash,
        "pov_left"_hash,
        "pov_right"_hash,
        "pov_up"_hash,
        "pov_down"_hash,
        "left_stick_click"_hash,
        "right_stick_click"_hash
    };
};

}