#pragma once

#include <algorithm>
#include <vector>
#include "StringHelperFunctions.h"
#include "tinyxml2.h"
#include "InputAction.h"

class InputState
{
public:
    InputState() {  }
    ~InputState() {}

    void addStandardAction(StandardInputAction action)
    {
        m_inputState.push_back(action);
    }

    const float getActionValue(const InputActions::ActionType& action) const
    {
        InputStateMap::const_iterator it = std::find_if(m_inputState.cbegin(), m_inputState.cend(), [action](const StandardInputAction& standardInput)
        {
            if (action.getType() == standardInput.getAction().getType())
            {
                return true;
            }

            return false;
        });
        if (it != m_inputState.end())
            return it->getValue();
        return false;
    }

    void setActionValue(const InputActions::ActionType& action, float value)
    {
        InputStateMap::iterator it = std::find_if(m_inputState.begin(), m_inputState.end(), [action](const StandardInputAction& standardInput)
        {
            if (action.getType() == standardInput.getAction().getType())
            {
                return true;
            }

            return false;
        });
        if (it != m_inputState.end())
             it->setValue(value);
    }

    void mergeInputState(const InputState& input)
    {
        for (size_t counter = 0; counter < m_inputState.size(); ++counter)
        {
            m_inputState[counter].setValue(input.m_inputState[counter].getValue() > 0 ? input.m_inputState[counter].getValue() : m_inputState[counter].getValue());
        }
    }
private:
    typedef std::vector< StandardInputAction> InputStateMap; //There is no reason for this class
    InputStateMap m_inputState;
};
