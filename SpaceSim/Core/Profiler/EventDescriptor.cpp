#include "EventDescriptor.h"

#include <string>

namespace Profiling
{

///-----------------------------------------------------------------------------
///! @brief  Compute a hash based on the event name and file and function name
///! @remark This should only be done once and then just stored in a static
///-----------------------------------------------------------------------------
size_t Profiling::EventDescriptor::computeHash() const
{
    std::string sourceStr = m_name;
    sourceStr += m_fileName;
    sourceStr += m_functionName;

    m_hash = 2166136261;
    for (size_t counter = 0; counter < sourceStr.size(); ++counter)
    {
        char c = sourceStr[counter];
        if (c >= 'A' && c <= 'Z')
        {
            c = c + ('a' - 'A');
        }
        m_hash = m_hash ^ c;
        m_hash = m_hash * 16777619;
    }

    return m_hash;
}

}