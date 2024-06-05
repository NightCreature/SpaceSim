#pragma once

template<class Function>
class FunctionWrapper
{
public:
    explicit FunctionWrapper(Function func) : m_function(func) {}

    bool operator()(size_t index)
    {
        return m_function. template operator()(index);
    }

private:

    Function m_function;
};
