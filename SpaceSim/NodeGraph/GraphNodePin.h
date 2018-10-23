#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"
#include "NodeGraph/GraphNode.h"

namespace NodeGraph
{

class PinDefinition
{
public:
    enum class Type : int
    {
        Bool,
        Int,
        Float,
        Double,
        Vector2,
        Vector3,

        Count
    };

    PinDefinition() = default;
    PinDefinition(Type type, bool isInPin) : m_type(type), m_isInPin(isInPin) {}

    Type GetType() const { return m_type; }
    bool IsInPin() const { return m_isInPin; }
private:
    Type m_type;
    bool m_isInPin;
};

class PinBase
{
public:
    PinBase() = default;
    virtual ~PinBase() {}
    
    PinDefinition::Type GetPinType() const { return m_pinDefinition.GetType(); }
protected:
    //Could carry type id
    PinDefinition m_pinDefinition;
};

template<class T>
class Pin : public PinBase
{
public:
    Pin(const PinDefinition& definition) { m_pinDefinition = definition; }
    virtual ~Pin() {}

    void SetValue(const T& value) { m_value = value; }
    const T& GetValue() const { return m_value; }

    HASH_ELEMENT_DEFINITION(Pin);
private:
    T m_value;
};

}
