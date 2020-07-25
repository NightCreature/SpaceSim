#include "profiler.h"

#include "Core/StringOperations/StringHelperFunctions.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <locale>
#include <sstream>
#include <any>

namespace Profiling
{

const size_t numberOfEventsBeforeFlushToFile = 1000000;

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Profiler::Profiler() : m_fileNameForSession(""), m_shouldFlushToFile(0), m_firstFlush(true)
{
    InitializeCriticalSection(&m_criticalSection);

    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    std::stringstream fileNameForSession;
    fileNameForSession << "profiling";
    fileNameForSession << std::put_time(std::localtime(&now), "%F %T");
    fileNameForSession << ".prf";

    m_fileNameForSession = fileNameForSession.str();

    m_fileNameForSession.erase(
        std::remove_if(m_fileNameForSession.begin(), m_fileNameForSession.end(),
            [](char& c) { return std::isspace<char>(c, std::locale::classic()); })
        , m_fileNameForSession.end());
    m_fileNameForSession.erase(
        std::remove_if(m_fileNameForSession.begin(), m_fileNameForSession.end(),
            [](char& c) { return c == ':'; })
        , m_fileNameForSession.end());
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
size_t Profiler::RegisterProfilingEventDescription(const EventDescriptor& eventDescriptor)
{
    EnterCriticalSection(&m_criticalSection);
    size_t retVal = eventDescriptor.computeHash();
    m_eventDescriptors.push_back(eventDescriptor);
    LeaveCriticalSection(&m_criticalSection);
    return retVal;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Event* Profiler::CaptureEvent(const size_t& eventDescriptorHash)
{
    EnterCriticalSection(&m_criticalSection);
    m_currentFrameCapture->m_events.push_back(Event(eventDescriptorHash));
    m_currentFrameCapture->m_eventHistogram[eventDescriptorHash]++;
    Event * event = &m_currentFrameCapture->m_events[m_currentFrameCapture->m_events.size() - 1];
    LeaveCriticalSection(&m_criticalSection);

    return event;
}

///-----------------------------------------------------------------------------
///! @brief Frame marker so we can track per frame things.
///! @remark This has a small overhead when we need to flush data to a file at about 32MB of memory use.
///-----------------------------------------------------------------------------
void Profiler::BeginFrame()
{
    EnterCriticalSection(&m_criticalSection);
    if (m_shouldFlushToFile >= numberOfEventsBeforeFlushToFile)
    {
        FlushToFile();
    }

    m_frames.push_back(FrameCapture());
    m_currentFrameCapture = &(m_frames[m_frames.size() - 1]);
    LeaveCriticalSection(&m_criticalSection);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void Profiler::Cleanup()
{
    DeleteCriticalSection(&m_criticalSection);

    FlushToFile();
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void Profiler::FlushToFile()
{
    std::fstream outputFile;
    std::ios_base::openmode openMode = std::ios_base::out | std::ios_base::in;
    
    std::string fileName;// = "/ProfileCaptures/";
    fileName += m_fileNameForSession;

    fileName = "ProfileCaptures.prf";


    outputFile.open(fileName, openMode); //Should probably grab this from the paths thing but that would require profiler to know about resource
    if (outputFile.is_open())
    {
        //begin object
        outputFile << "{\n";
        
        if (m_firstFlush) //Stream out the descriptors we need these to track what each event means in the file
        {
            
            outputFile << "\"TimerResolution\": " <<  m_timer.getResolution() << ",\n";

            outputFile << "\"EventDescriptors\": [\n";
            size_t counter = 0;
            for (auto& descriptor : m_eventDescriptors)
            {
                descriptor.toJson(outputFile);
                outputFile << (counter < m_eventDescriptors.size() ? ",\n" : "\n");
                ++counter;
            }
            outputFile << "]\n";
        }

        outputFile << "\"Frames\": [\n";
        size_t counter = 0;
        for (auto& frameCaputre : m_frames)
        {
            frameCaputre.toJson(outputFile);
            outputFile << (counter < m_frames.size() ? ",\n" : "\n");
            ++counter;
        }
        
        outputFile << "]\n";
        //Finish the whole object
        outputFile << "}";

        outputFile.close();

        //m_eventDescriptors.clear();
        //m_frames.clear();
    }
    else
    {
        MSG_TRACE("Failed to open File stream with error: %s", std::strerror(errno));
    }
}

void FrameCapture::toJson(std::fstream& stream)
{
    stream << "{\n";

    stream << "\"Events\": [\n";
    for (size_t counter = 0; counter < m_events.size(); ++counter)
    {
        m_events[counter].toJson(stream);
        stream << (counter < m_events.size() ? ",\n" : "\n");
    }
    stream << "],\n";

    stream << "\"Histogram\": [\n";
    size_t histoCounter = 0;
    for (auto& histogram : m_eventHistogram)
    {
        stream << "{\n";
        stream << "\"Count\": " << histogram.second << ",\n";
        stream << "\"Event Hash\": " << histogram.first << "\n";
        stream << "}" << (histoCounter < m_eventHistogram.size() ? ",\n" :"\n");
        ++histoCounter;
    }
    stream << "]";

    stream << "}";
}

}