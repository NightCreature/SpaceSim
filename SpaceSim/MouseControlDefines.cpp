#include "MouseControlDefines.h"
#include "StringHelperFunctions.h"

namespace Input
{

MouseControlDefinitions::MouseControlDefinitions()
{
    m_MouseToAction = NameHashToMouseInput();
    m_MouseToAction.insert(NameHashToMouseInputPair(hashString("positive_x"), PositiveX));
    m_MouseToAction.insert(NameHashToMouseInputPair(hashString("negative_x"), NegativeX));
    m_MouseToAction.insert(NameHashToMouseInputPair(hashString("positive_y"), PositiveY));
    m_MouseToAction.insert(NameHashToMouseInputPair(hashString("negative_y"), NegativeY));
    m_MouseToAction.insert(NameHashToMouseInputPair(hashString("button_1"), MouseButton1));
    m_MouseToAction.insert(NameHashToMouseInputPair(hashString("button_2"), MouseButton1));
    m_MouseToAction.insert(NameHashToMouseInputPair(hashString("button_3"), MouseButton1));
    m_MouseToAction.insert(NameHashToMouseInputPair(hashString("button_4"), MouseButton1));
    m_MouseToAction.insert(NameHashToMouseInputPair(hashString("button_5"), MouseButton1));
    m_MouseToAction.insert(NameHashToMouseInputPair(hashString("wheel_up"), MouseWheelUp));
    m_MouseToAction.insert(NameHashToMouseInputPair(hashString("wheel_down"), MouseWheelDown));
}

}

