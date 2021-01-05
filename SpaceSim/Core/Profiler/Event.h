#pragma once

#include <fstream>
#include <windows.h>

namespace Profiling
{

class Event
{
public:
    Event(size_t eventDescriptorHash) : m_beginTimeStamp(0), m_endTimeStamp(0), m_descriptorHash(eventDescriptorHash), m_threadId(static_cast<size_t>(GetCurrentThreadId())) {}
    ~Event() {}

    void toJson(std::fstream& stream)
    {
        stream << "{\n";
        stream << "\"End Timestamp\": " << m_endTimeStamp << ",\n";
        stream << "\"Event Hash\" : "<< m_descriptorHash <<",\n";
        stream << "\"Start Timestamp\" : " << m_beginTimeStamp << ",\n";
        stream << "\"ThreadID\" : " << m_threadId << "\n";
        stream << "}";
    }

    size_t m_beginTimeStamp;
    size_t m_endTimeStamp;
    size_t m_descriptorHash;
    size_t m_threadId;
};

class EventWrapper
{
public:
    EventWrapper(size_t eventDescriptorHash);
    ~EventWrapper();

private:
    Event* m_event;
};
}