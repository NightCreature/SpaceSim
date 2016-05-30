#include "Input.h"

#ifdef _DEBUG
//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void Input::printInput()
{
    for (auto inputState : m_input)
    {
        inputState->printState();
    }
}
#endif

