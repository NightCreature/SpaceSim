#pragma once

#include <string>
#include <vector>

class HttpMessageBuffer
{
public:

    //-----------------------------------------------------------------------------
    //! @brief   TODO enter a description
    //! @remark
    //-----------------------------------------------------------------------------
    void AddMessages(const std::vector< std::string >& buffer)
    {
        for (auto message : buffer)
        {
            m_buffer.push_back(message);
        }
    }

private:
    std::vector<std::string> m_buffer;

};