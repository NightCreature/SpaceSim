#include "InputState.h"


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
        MSG_TRACE_CHANNEL( "Input State", "Input state for action %s is value %f ", inputAction.getAction().getNameString().c_str(), inputAction.getValue());
    }
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void InputState::mergeInputState(const InputState& input)
{
    static bool test = false;
    if (test)
    {
        printState();
    }
    for (size_t counter = 0; counter < m_inputState.size(); ++counter)
    {
        for (auto standardInputAction : input.m_inputState)
        {
            if (standardInputAction.getAction().getType() == m_inputState[counter].getAction().getType())
            {
                float currentValue = m_inputState[counter].getValue();
                bool currentSign = currentValue > 0.0f;
                float inputValue = standardInputAction.getValue();
                bool inputSign = inputValue > 0.0f;
                if (currentSign == inputSign)
                {
                    //same sign, use max value, values should be clamped to 1.0f max anyway
                    currentValue = currentValue > inputValue ? currentValue : inputValue;
                   
                }
                else
                {
                    //Different signs, take the difference, need to figure out which one is bigger or negative
                    currentValue = currentValue > 0.0f ? (inputValue - currentValue) : (currentValue - inputValue);
                }
#ifdef _DEBUG
                if (test)
                {
                    MSG_TRACE_CHANNEL("Input State", "During merging %s value will change to %f", standardInputAction.getAction().getNameString().c_str(), currentValue);
                }
#endif

                m_inputState[counter].setValue(currentValue);
            }
        }
    }
    if (test)
    {
        printState();
        input.printState();
    }
}

#endif