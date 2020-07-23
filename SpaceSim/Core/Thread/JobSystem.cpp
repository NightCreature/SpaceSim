#include "JobSystem.h"

#include <atomic>
#include <sstream>

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
JobSystem::JobSystem(size_t numThreads)
{
    m_workerThreads.resize(numThreads);
    m_numberOfSleepingThreads = 0;

    m_eventHandle = CreateEvent(NULL, TRUE, FALSE, "SignalWorkAvailable");
    m_workFinishedEvent = CreateEvent(NULL, TRUE, FALSE, "WorkFinishedEvent");

    size_t index = 0;
    for (auto& threadStatus : m_workerThreads)
    {
        threadStatus.m_thread.SetJobsystem(this, index, m_eventHandle);
        threadStatus.m_working = false;
        ++index;
    }

    //Set the work available even to unsignaled
    ResetEvent(m_eventHandle);
    ResetEvent(m_workFinishedEvent);

    //Start all threads
    index = 0;
    for (auto& threadStatus : m_workerThreads)
    {
        std::stringstream str;
        str << "WorkerThread" << index;
        threadStatus.m_working = true;
        threadStatus.m_thread.createThread(1024*1024, str.str());
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
JobSystem::~JobSystem()
{
    for (auto& threadStatus : m_workerThreads)
    {
        threadStatus.m_thread.stopThread();
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void JobSystem::WorkerThreadSleeping(size_t index)
{
    std::scoped_lock<std::mutex> sl(m_finishedMutex);
    m_workerThreads[index].m_working = false;
    ++m_numberOfSleepingThreads;

    //Should we set the event for all work is done
    if (m_jobQueue.m_jobs.empty() && m_numberOfSleepingThreads == m_workerThreads.size())
    {
        SetEvent(m_workFinishedEvent);
    }

}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void JobSystem::WorkerThreadActive(size_t index)
{
    std::scoped_lock<std::mutex> sl(m_finishedMutex);
    m_workerThreads[index].m_working = true;
    --m_numberOfSleepingThreads;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void JobSystem::SignalWorkAvailable()
{
    SetEvent(m_eventHandle);
    ResetEvent(m_workFinishedEvent);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void JobSystem::WaitfForJobsToFinish()
{
    std::stringstream str("");
    str << "<<<<< JobSystem >>>>>\n";
    str << "Number of sleeping threads: " << m_numberOfSleepingThreads << "\n";
    str << "Number of WorkerThreads: " << m_workerThreads.size() << "\n";
    str << "Number of tasks: " << m_jobQueue.m_jobs.size() << "\n";
    str << "<<<<< JobSystem >>>>>\n";
    OutputDebugString(str.str().c_str());

    ResetEvent(m_workFinishedEvent);
    SetEvent(m_eventHandle);
    DWORD waitReturn = WaitForSingleObject(m_workFinishedEvent, INFINITE);

    str.str("");
    str << "<<<<< JobSystem >>>>>\n";
    str << "Number of sleeping threads: " << m_numberOfSleepingThreads << "\n";
    str << "Number of WorkerThreads: " << m_workerThreads.size() << "\n";
    str << "Number of tasks: " << m_jobQueue.m_jobs.size() << "\n";
    str << "waitReturn: " << waitReturn << "\n";
    str << "<<<<< JobSystem >>>>>\n";
    OutputDebugString(str.str().c_str());
}
