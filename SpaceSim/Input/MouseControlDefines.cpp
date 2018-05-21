#include "Input/MouseControlDefines.h"
#include "Core/StringOperations/StringHelperFunctions.h"

namespace InputDefines
{

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
MouseControlDefinitions::MouseControlDefinitions()
{
    m_inputNameHash[static_cast<unsigned int>(MouseInput::PositiveX)] = hashString("positive_x");
    m_inputNameHash[static_cast<unsigned int>(MouseInput::NegativeX)] = hashString("negative_x");
    m_inputNameHash[static_cast<unsigned int>(MouseInput::PositiveY)] = hashString("positive_y");
    m_inputNameHash[static_cast<unsigned int>(MouseInput::NegativeY)] = hashString("negative_y");
    m_inputNameHash[static_cast<unsigned int>(MouseInput::MouseButton1)] = hashString("button_1");
    m_inputNameHash[static_cast<unsigned int>(MouseInput::MouseButton1)] = hashString("button_2");
    m_inputNameHash[static_cast<unsigned int>(MouseInput::MouseButton1)] = hashString("button_3");
    m_inputNameHash[static_cast<unsigned int>(MouseInput::MouseButton1)] = hashString("button_4");
    m_inputNameHash[static_cast<unsigned int>(MouseInput::MouseButton1)] = hashString("button_5");
    m_inputNameHash[static_cast<unsigned int>(MouseInput::MouseWheelUp)] = hashString("wheel_up");
    m_inputNameHash[static_cast<unsigned int>(MouseInput::MouseWheelDown)] = hashString("wheel_down");
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
MouseControlDefinitions::MouseInput MouseControlDefinitions::FindControllerInputIdForHashId(size_t hashId) const
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

}

