#include "XControllerDefines.h"

#include "StringHelperFunctions.h"

namespace Input
{

XControllerDefines::XControllerDefines()
{
    m_XInputToAction = NameHashToXInput();

    m_XInputToAction.insert(NameHashToXInputPair(hashString("left_stick_positive_x"), LeftStickPositiveX));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("left_stick_negative_x"), LeftStickNegativeX));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("left_stick_positive_y"), LeftStickPositiveY));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("left_stick_negative_y"), LeftStickNegativeY));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("right_stick_positive_x"), RightStickPositiveX));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("right_stick_negative_x"), RightStickNegativeX));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("right_stick_positive_y"), RightStickPositiveY));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("right_stick_negative_y"), RightStickNegativeY));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("left_trigger"), LeftTrigger));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("right_trigger"), RightTrigger));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("left_shoulder_button"), LeftShoulderButton));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("right_shoulder_button"), RightShoulderButton));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("back_button"), BackButton));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("start_button"), StartButton));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("a_button"), AButton));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("b_button"), BBUtton));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("x_button"), XButton));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("y_button"), YButton));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("pov_left"), POVLeft));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("pov_right"), POVRight));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("pov_up"), POVUp));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("pov_down"), POVDown));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("left_stick_click"), LeftStickClick));
    m_XInputToAction.insert(NameHashToXInputPair(hashString("right_stick_click"), RightStickClick));
}

}