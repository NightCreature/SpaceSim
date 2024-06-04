#include "JobSystem.h"

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Logging/LoggingMacros.h"

#include <atomic>
#include <sstream>
#include "Core/Types/TypeHelpers.h"
#include "Core/Resource/Resourceable.h"
#include "../Profiler/ProfilerMacros.h"


///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
JobSystem::JobSystem(size_t numThreads)
{
    m_workerThreads.resize(numThreads);
    m_numberOfSleepingThreads = 0;

    m_workAvaliable = CreateEvent(NULL, TRUE, FALSE, "SignalWorkAvailable");
    m_workFinishedEvent = CreateEvent(NULL, TRUE, FALSE, "WorkFinishedEvent");

    size_t index = 0; //this starts at one so we can use this thread as a working thread too
    for (auto& threadStatus : m_workerThreads)
    {
        threadStatus.m_thread.SetJobsystem(this, index, m_workAvaliable);
        threadStatus.m_working = false;
        ++index;
    }

    //Set the work available even to unsignaled
    ResetEvent(m_workAvaliable);
    ResetEvent(m_workFinishedEvent);

    //Start all threads
    index = 0;
    for (auto& threadStatus : m_workerThreads)
    {
        std::stringstream str;
        threadStatus.m_threadIndex = index;
        if (index == 0)
        {
            str << "JobSystemThread";
        }
        else
        {
            str << "WorkerThread" << index;
            threadStatus.m_working = true;
            threadStatus.m_thread.createThread(1024 * 1024, str.str());
        }
        ++index;
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
JobSystem::~JobSystem()
{
    //in case threads are sleeping flag the conditon that work is available


    for (auto& threadStatus : m_workerThreads)
    {
        threadStatus.m_thread.killThread();
        SetEvent(m_workAvaliable);
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

    if (m_jobQueue.m_jobs.empty())
    {
        ResetEvent(m_workAvaliable);
    }

    
    m_workerThreads[index - 1].m_working = false; //index starts at 1
    ++m_numberOfSleepingThreads;

    //Should we set the event for all work is done
    if (m_jobQueue.m_jobs.empty())
    {
        if (m_numberOfSleepingThreads == m_workerThreads.size())
        {
            SetEvent(m_workFinishedEvent);
        }
    }

}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void JobSystem::WorkerThreadActive(size_t index)
{
    std::scoped_lock<std::mutex> sl(m_finishedMutex);
    m_workerThreads[index - 1].m_working = true;
    --m_numberOfSleepingThreads;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void JobSystem::SignalWorkAvailable()
{
    if (!m_jobQueue.m_jobs.empty())
    {
        SetEvent(m_workAvaliable);
        ResetEvent(m_workFinishedEvent);
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void JobSystem::WaitfForJobsToFinish()
{
    //std::stringstream str("");
    //str << "<<<<< JobSystem >>>>>\n";
    //str << "Number of sleeping threads: " << m_numberOfSleepingThreads << "\n";
    //str << "Number of WorkerThreads: " << m_workerThreads.size() << "\n";
    //str << "Number of tasks: " << m_jobQueue.m_jobs.size() << "\n";
    //str << "<<<<< JobSystem >>>>>\n";
    //OutputDebugString(str.str().c_str());

    ResetEvent(m_workFinishedEvent);
    if (!m_jobQueue.m_jobs.empty())
    {
        //Jobqeue is not empty so set it to process the jobs
        SetEvent(m_workAvaliable);
    }
    DWORD waitReturn = WaitForSingleObject(m_workFinishedEvent, INFINITE);
    UNUSEDPARAM(waitReturn);

    //str.str("");
    //str << "<<<<< JobSystem >>>>>\n";
    //str << "Number of sleeping threads: " << m_numberOfSleepingThreads << "\n";
    //str << "Number of WorkerThreads: " << m_workerThreads.size() << "\n";
    //str << "Number of tasks: " << m_jobQueue.m_jobs.size() << "\n";
    //str << "waitReturn: " << waitReturn << "\n";
    //str << "<<<<< JobSystem >>>>>\n";
    //OutputDebugString(str.str().c_str());
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void JobSystem::ProcessWork()
{
    PROFILE_FUNCTION();
    ResetEvent(m_workFinishedEvent);
    if (!m_jobQueue.m_jobs.empty())
    {
        SetEvent(m_workAvaliable);
    }

    //pick up a job here until there is nothing left
    auto workLoad = m_jobQueue.GetNextWorkLoad();
    while (!workLoad.m_empty)
    {
        //Execute this workload
        
        workLoad.m_job->Execute(&m_workerThreads[0]); //running on the main thread
        workLoad.m_job->FinishJob();
        //once we are done with the work load get ride of it, this should probably be done better
        //delete workLoad.m_job;
        workLoad = m_jobQueue.GetNextWorkLoad();
    }

    //at the end here the job queue should be empty if all threads are sleeping
    if (m_numberOfSleepingThreads == m_workerThreads.size() && !m_jobQueue.m_jobs.empty())
    {
        //Something is wrong
        MSG_TRACE_CHANNEL("[JOBSYSTEM]", "There is work left in the job queue there shouldn't be anything there at this point, amount of work left: %d", m_jobQueue.m_jobs.size());
    }
    else
    {
        DWORD waitReturn = WaitForSingleObject(m_workFinishedEvent, INFINITE);
        UNUSEDPARAM(waitReturn);
    }
}

void JobSystem::SetGameResource(Resource* resource)
{
    for (auto& threadStatus : m_workerThreads)
    {
        threadStatus.m_gameResource = resource;
    }
}

void JobSystem::SetRenderResource(Resource* resource)
{
    for (auto& threadStatus : m_workerThreads)
    {
        threadStatus.m_renderResource = resource;
    }

}
