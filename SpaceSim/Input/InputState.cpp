#include "Input/InputState.h"
#include "Math/MathUtilityFunctions.h"
#include "Input/InputSystem.h"

#ifdef _DEBUG
///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void InputState::printState() const
{
    MSG_TRACE_CHANNEL("Input State", "/**********************INPUT STATE DATA*********************************/");
    for (auto inputAction : m_inputState)
    {
        MSG_TRACE_CHANNEL( "Input State", "Input state for action %s is value %f ", InputSystem::m_actionNames[inputAction.getAction()].c_str(), inputAction.getValue());
    }
}
#endif

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void InputState::mergeInputState(const InputState& input)
{
#ifdef _DEBUG
    static bool test = false;
    if (test)
    {
        printState();
    }
#endif
    for (auto standardInputAction : input.m_inputState)
    {
        //Does this action exist in the current input state?
        InputStateMap::iterator it = std::find_if(m_inputState.begin(), m_inputState.end(), [standardInputAction](const StandardInputAction& inputAction) { return inputAction.getAction().getType() == standardInputAction.getAction().getType(); });
        if (it != m_inputState.end())
        {
            float currentValue = it->getValue();
            float inputValue = standardInputAction.getValue();
            //Different signs, take the difference, need to figure out which one is bigger or negative
            currentValue = currentValue + inputValue;
            currentValue = math::clamp(currentValue, 0.0f, 1.0f);
#ifdef _DEBUG
            if (test)
            {
                MSG_TRACE_CHANNEL("Input State", "During merging %s value will change to %f", InputSystem::m_actionNames[standardInputAction.getAction()].c_str(), currentValue);
            }
#endif

            it->setValue(currentValue);
        }
        else
        {
            //Value is not found in the current input set so we should add it to it.
            m_inputState.push_back(standardInputAction);
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

