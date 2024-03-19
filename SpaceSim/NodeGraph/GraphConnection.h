#pragma once

#include "Core/Types/Types.h"

namespace NodeGraph
{
class PinBase; 

///Represesents a data channel it just stores the bytes the data is.
class GraphDataChannel
{
public:
    GraphDataChannel() = default;

    template<class T>
    void SetValue(const T& value)
    {
        m_size = sizeof(value);
        m_data = new byte[m_size];
        memcpy(static_cast<void*>(m_data), static_cast<void*>(&value), m_size);
    }
    template<class T>
    const T* GetValue() { return static_cast<const T*>(m_data); }

private:
    byte* m_data;
    size_t m_size;
};

class GraphConnection
{
public:
    GraphConnection(const PinBase* lhs, const PinBase* rhs) : m_lhs(lhs), m_rhs(rhs), m_data({}) {}
    ~GraphConnection() = default;

    template<class T>
    void SetValue(const T& value) { m_data.SetValue(value); }
private:
    const PinBase* m_lhs;
    const PinBase* m_rhs;

    const GraphDataChannel m_data;
};
}