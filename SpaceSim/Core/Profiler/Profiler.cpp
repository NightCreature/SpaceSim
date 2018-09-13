#include "profiler.h"

#include "Core/StringOperations/StringHelperFunctions.h"

#include <nlohmann\json.hpp>

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <locale>
#include <sstream>

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
    fileNameForSession<< "profiling";
    fileNameForSession << std::put_time(std::localtime(&now), "%F %T");
    fileNameForSession << ".prf";

    m_fileNameForSession = fileNameForSession.str();

    m_fileNameForSession.erase(
        std::remove_if(m_fileNameForSession.begin(), m_fileNameForSession.end(),
            [](char &c) { return std::isspace<char>(c, std::locale::classic()); })
        , m_fileNameForSession.end());
    m_fileNameForSession.erase(
        std::remove_if(m_fileNameForSession.begin(), m_fileNameForSession.end(),
            [](char &c) { return c == ':'; })
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
    std::ofstream outputFile;
    std::ios_base::openmode openMode = std::ios_base::out;
    if (!m_firstFlush)
    {
        openMode |= std::ios_base::app;
    }
    
    std::string fileName;// = "/ProfileCaptures/";
    fileName += m_fileNameForSession;

    fileName = "ProfileCaptures.prf";

    outputFile.open(fileName, openMode); //Should probably grab this from the paths thing but that would require profiler to know about resource
    if (outputFile.is_open())
    {
        nlohmann::json json_data;
        
        if (m_firstFlush) //Stream out the descriptors we need these to track what each event means in the file
        {
            m_firstFlush = false;
            for (auto& descriptor : m_eventDescriptors)
            {
                nlohmann::json eventDescriptor;
                eventDescriptor["Event Name"] = descriptor.getEventName();
                eventDescriptor["File Name"] = descriptor.getFileName();
                eventDescriptor["Function Name"] = descriptor.getFunctioName();
                eventDescriptor["Line Number"] = descriptor.getLineNumber();
                eventDescriptor["Event Hash"] = descriptor.getHash();
                json_data["EventDescriptors"].push_back(eventDescriptor);
            }

            json_data["TimerResolution"] = m_timer.getResolution();
        }
        //else
        //{
        //    json_data << outputFile;
        //}

        nlohmann::json framesArray;

        for (auto& frameCaputre : m_frames)
        {
            nlohmann::json frameObject;
            for (auto& event : frameCaputre.m_events)
            {
                nlohmann::json eventData;
                eventData["Event Hash"] = event.m_descriptorHash;
                eventData["Start Timestamp"] = event.m_beginTimeStamp;
                eventData["End Timestamp"] = event.m_endTimeStamp;
                frameObject["Events"].push_back(eventData);
            }

            for (auto& histogram : frameCaputre.m_eventHistogram)
            {
                nlohmann::json histogramData;
                histogramData["Event Hash"] = histogram.first;
                histogramData["Count"] = histogram.second;
                frameObject["Histogram"].push_back(histogramData);
            }

            framesArray.push_back(frameObject);
        }

        json_data["Frames"] = framesArray;

        outputFile << std::setw(4) << json_data;

        outputFile.close();

        //m_eventDescriptors.clear();
        //m_frames.clear();
    }
    else
    {
        MSG_TRACE("Failed to open File stream with error: %s", std::strerror(errno));
    }
}

}