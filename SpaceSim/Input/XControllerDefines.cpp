#include "Input/XControllerDefines.h"

#include "Core/StringOperations/StringHelperFunctions.h"

#include <algorithm>

namespace InputDefines
{
///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
XControllerDefines::XControllerDefines()
{
    m_inputNameHash[LeftStickPositiveX] = hashString("left_stick_positive_x");
    m_inputNameHash[LeftStickNegativeX] = hashString("left_stick_negative_x");
    m_inputNameHash[LeftStickPositiveY] = hashString("left_stick_positive_y");
    m_inputNameHash[LeftStickNegativeY] = hashString("left_stick_negative_y");
    m_inputNameHash[RightStickPositiveX] = hashString("right_stick_positive_x");
    m_inputNameHash[RightStickNegativeX] = hashString("right_stick_negative_x");
    m_inputNameHash[RightStickPositiveY] = hashString("right_stick_positive_y");
    m_inputNameHash[RightStickNegativeY] = hashString("right_stick_negative_y");
    m_inputNameHash[LeftTrigger] = hashString("left_trigger");
    m_inputNameHash[RightTrigger] = hashString("right_trigger");
    m_inputNameHash[LeftShoulderButton] = hashString("left_shoulder_button");
    m_inputNameHash[RightShoulderButton] = hashString("right_shoulder_button");
    m_inputNameHash[BackButton] = hashString("back_button");
    m_inputNameHash[StartButton] = hashString("start_button");
    m_inputNameHash[AButton] = hashString("a_button");
    m_inputNameHash[BBUtton] = hashString("b_button");
    m_inputNameHash[XButton] = hashString("x_button");
    m_inputNameHash[YButton] = hashString("y_button");
    m_inputNameHash[POVLeft] = hashString("pov_left");
    m_inputNameHash[POVRight] = hashString("pov_right");
    m_inputNameHash[POVUp] = hashString("pov_up");
    m_inputNameHash[POVDown] = hashString("pov_down");
    m_inputNameHash[LeftStickClick] = hashString("left_stick_click");
    m_inputNameHash[RightStickClick] = hashString("right_stick_click");
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
XControllerDefines::XControllerInput XControllerDefines::FindControllerInputIdForHashId(unsigned int hashId) const
{
    for (short counter = 0; counter < NumInputActions; ++counter)
    {
        if (m_inputNameHash[counter] == hashId)
        {
            return static_cast<XControllerInput>(counter);
        }
    }

    return NumInputActions;
}

}