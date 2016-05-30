#include "Input/MouseControlDefines.h"
#include "Core/StringOperations/StringHelperFunctions.h"

namespace InputDefines
{

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
MouseControlDefinitions::MouseControlDefinitions()
{
    m_inputNameHash[PositiveX] = hashString("positive_x");
    m_inputNameHash[NegativeX] = hashString("negative_x");
    m_inputNameHash[PositiveY] = hashString("positive_y");
    m_inputNameHash[NegativeY] = hashString("negative_y");
    m_inputNameHash[MouseButton1] = hashString("button_1");
    m_inputNameHash[MouseButton1] = hashString("button_2");
    m_inputNameHash[MouseButton1] = hashString("button_3");
    m_inputNameHash[MouseButton1] = hashString("button_4");
    m_inputNameHash[MouseButton1] = hashString("button_5");
    m_inputNameHash[MouseWheelUp] = hashString("wheel_up");
    m_inputNameHash[MouseWheelDown] = hashString("wheel_down");
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
MouseControlDefinitions::MouseInput MouseControlDefinitions::FindControllerInputIdForHashId(unsigned int hashId) const
{
    for (short counter = 0; counter < NumInputActions; ++counter)
    {
        if (m_inputNameHash[counter] == hashId)
        {
            return static_cast<MouseInput>(counter);
        }
    }

    return NumInputActions;
}

}

