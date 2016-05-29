#include "InputState.h"
#include "MathUtilityFunctions.h"
#include "InputSystem.h"

#ifdef _DEBUG
//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void InputState::printState() const
{
    MSG_TRACE_CHANNEL("Input State", "/**********************INPUT STATE DATA*********************************/");
    for (auto inputAction : m_inputState)
    {
        MSG_TRACE_CHANNEL( "Input State", "Input state for action %s is value %f ", InputSystem::m_actionNames[inputAction.getAction()], inputAction.getValue());
    }
}
#endif

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void InputState::mergeInputState(const InputState& input)
{
#ifdef _DEBUG
    static bool test = false;
    if (test)
    {
        printState();
    }
#endif
    for (size_t counter = 0; counter < m_inputState.size(); ++counter)
    {
        for (auto standardInputAction : input.m_inputState)
        {
            if (standardInputAction.getAction().getType() == m_inputState[counter].getAction().getType())
            {
                float currentValue = m_inputState[counter].getValue();
                float inputValue = standardInputAction.getValue();
                //Different signs, take the difference, need to figure out which one is bigger or negative
                currentValue = currentValue + inputValue;
                currentValue = math::clamp(currentValue, 0.0f, 1.0f);
#ifdef _DEBUG
            if (test)
            {
                MSG_TRACE_CHANNEL("Input State", "During merging %s value will change to %f", InputSystem::m_actionNames[standardInputAction.getAction()], currentValue);
            }
#endif

                m_inputState[counter].setValue(currentValue);
            }
        }
    }

#ifdef _DEBUG
    if (test)
    {
        printState();
        input.printState();
    }
#endif
}

