#pragma once

#include <map>
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
        m_inputState.insert(InputStatePair(action.getAction(), action));
    }

    const float getActionValue(InputActions::ActionType action) const
    {
        InputStateMap::const_iterator it = m_inputState.find(action);
        if (it != m_inputState.end())
            return it->second.getValue();
        return false;
    }

    void setActionValue(InputActions::ActionType action, float value)
    {
        if (m_inputState.find(action) != m_inputState.end())
             m_inputState[action].setValue(value);
    }
private:
    typedef std::map<InputActions::ActionType, StandardInputAction> InputStateMap; //There is no reason for this class
    typedef std::pair<InputActions::ActionType, StandardInputAction> InputStatePair;
    InputStateMap m_inputState;
};
