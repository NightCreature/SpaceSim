#pragma once

//DirtyValue
template<class T>
class DirtyValue
{
public:
    DirtyValue() = default;

    DirtyValue(const T& value)
    {
        m_currentValue = value;
        m_previousValue = value;
    }

    void SetValue(const T& value)
    {
        m_previousValue = m_currentValue;
        m_currentValue = value;
    }
    
    const T& GetValue() const { return m_currentValue; }
    const T& GetPreviousValue() const { return m_previousValue; }

    const T& operator()() const { return m_currentValue; }
    bool Changed() const { return m_currentValue != m_previousValue; }
private:
    T m_currentValue;
    T m_previousValue;

};