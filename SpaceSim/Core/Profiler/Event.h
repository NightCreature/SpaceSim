#pragma once

namespace Profiling
{

class Event
{
public:
    Event(size_t eventDescriptorHash) : m_beginTimeStamp(0), m_endTimeStamp(0), m_descriptorHash(eventDescriptorHash) {}
    ~Event() {}

    size_t m_beginTimeStamp;
    size_t m_endTimeStamp;
    size_t m_descriptorHash;
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