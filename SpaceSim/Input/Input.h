#pragma once

#include "InputState.h"
#include <vector>

//Need to assign input states to players to use

///-----------------------------------------------------------------------------
///! @brief   Container class to collect all input from all connected controllers
///! @remark
///-----------------------------------------------------------------------------
class Input
{
public:
    Input() {}
    ~Input() {}

    //We should merge the input states here so that you can use both keyboard and a controller at the same time
    void setInput(unsigned int index, InputState* input)
    {
        if (m_input.empty() || index > m_input.size())
        {
            m_input.push_back(input);
        }
        else
        {
            m_input[index]->mergeInputState(*input);
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

    size_t getNumberOfControllersInInput() { return m_input.size(); }
#ifdef _DEBUG
    void printInput();
#endif
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