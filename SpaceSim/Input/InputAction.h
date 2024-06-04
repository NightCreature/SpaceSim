#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"
#include <string>

namespace InputActions
{
    //enum ActionType
    //{
    //    eUnknownButtonType = -1,
    //    ePOVUp = 0,
    //    ePOVDown,
    //    ePOVLeft,
    //    ePOVRight,
    //    eAButton,
    //    eBButton,
    //    eXButton,
    //    eYButton,
    //    eStartButton,
    //    eBackButton,
    //    eLBButton,
    //    eRBButton,
    //    eLeftThumbButton,
    //    eRightThumbButton,
    //    eLeftXAxis,
    //    eLeftYAxis,
    //    eRightXAxis,
    //    eRightYAxis,
    //    eZAxis,
    //    eLeftTrigger,
    //    eRightTrigger
    //};

    class ActionType
    {
    public:
        ActionType() : m_actionType(0), m_actionTypeLngKey(0){}
        ActionType(const std::string& actionTypeName) : m_actionType(Hashing::hashString(actionTypeName)), m_actionTypeLngKey(0) {}
        ActionType(const std::string& actionTypeName, const std::string& lngActionName) : m_actionType(Hashing::hashString(actionTypeName)), m_actionTypeLngKey(Hashing::hashString(lngActionName.c_str())) {}

        size_t getType() const { return m_actionType; }
        size_t getLanguageString() const { return m_actionTypeLngKey; }
#if defined( DEBUG ) || defined( _DEBUG )
        //const std::string& getNameString() const { return m_actionTypeName; }
#endif
        bool operator < ( const ActionType& rhs ) const { return m_actionType < rhs.m_actionType; }
    private:
        size_t m_actionType;
        size_t m_actionTypeLngKey;
        //std::string m_actionTypeName;
    };
}

template<class ValueType, class InputActionType>
class InputAction
{
public:
    InputAction() : m_action(), m_value(0) {}
    InputAction(const InputActionType action) : m_action(action), m_value(0) {}
    ~InputAction() {}

    const InputActionType& getAction() const { return m_action; }
    const ValueType getValue() const { return m_value; }
    void setValue(const ValueType& value) { m_value = value; }
private:
    InputActionType m_action;
    ValueType m_value;
};

typedef InputAction<float, InputActions::ActionType> StandardInputAction;
