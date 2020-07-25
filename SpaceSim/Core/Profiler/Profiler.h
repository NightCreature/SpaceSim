#pragma once

#include "Core/Profiler/EventDescriptor.h"
#include "Core/Profiler/Event.h"
#include "Core/Thread/Timer.h"

#include <fstream>
#include <string>

#include <unordered_map>
#include <vector>

namespace Profiling
{

class EventDescriptor;

struct FrameCapture
{
    std::vector<Event> m_events;
    std::unordered_map<size_t, size_t> m_eventHistogram;

    void toJson(std::fstream& stream);
};

class Profiler
{
public:
    ~Profiler() {}

    static Profiler& GetInstance() { static Profiler instance; return instance; }

    size_t RegisterProfilingEventDescription( const EventDescriptor& eventDescriptor);
    Event* Profiler::CaptureEvent(const size_t& eventDescriptorHash);

    void BeginFrame();

    size_t GetTimeStamp() { return m_timer.getTimeStamp(); }
    void Cleanup();

    void WriteFile() { FlushToFile(); }
private:
    Profiler();
    void FlushToFile();

    CRITICAL_SECTION m_criticalSection;

    std::vector<EventDescriptor> m_eventDescriptors;
    std::vector<FrameCapture> m_frames;

    std::string m_fileNameForSession;

    PerformanceTimer m_timer;

    FrameCapture* m_currentFrameCapture;

    size_t m_shouldFlushToFile;
    bool m_firstFlush;
};

}