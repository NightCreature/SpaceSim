#include "Event.h"

#include "Core/Profiler/Profiler.h"

namespace Profiling
{

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
EventWrapper::EventWrapper(size_t eventDescriptorHash)
{
    m_event = Profiler::GetInstance().CaptureEvent(eventDescriptorHash);
    if (m_event != nullptr)
    {
        m_event->m_beginTimeStamp = Profiler::GetInstance().GetTimeStamp();
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
EventWrapper::~EventWrapper()
{
    if (m_event != nullptr)
    {
        m_event->m_endTimeStamp = Profiler::GetInstance().GetTimeStamp();
    }
}

}
