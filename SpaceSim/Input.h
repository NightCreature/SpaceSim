#pragma once

#include "InputState.h"
#include <vector>

//Need to assign input states to players to use

//-----------------------------------------------------------------------------
//! @brief   Container class to collect all input from all connected controllers
//! @remark
//-----------------------------------------------------------------------------
class Input
{
public:
    Input() {}
    ~Input() {}

    void setInput(unsigned int index, InputState* input)
    {
        if (index <= m_input.size())
        {
            m_input.push_back(input);
        }
        else
        {
            m_input[index] = input;
        }
    }

    const InputState* getInput(unsigned int index) const
    {
        if (index < m_input.size())
        {
            return m_input[index];
        }

        return 0;
    }
protected:
private:
    std::vector<InputState*> m_input;
};

class DummyInput : public Input
{
public:
    DummyInput() {}
    ~DummyInput() {}
};