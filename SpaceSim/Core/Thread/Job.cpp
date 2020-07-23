#include "Job.h"

#include <atomic>
#include <sstream>


///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
JobQueue::JobQueue()
{
    InitializeCriticalSectionAndSpinCount(&m_criticalSection, 50);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void JobQueue::AddJob(Job* job)
{
    std::scoped_lock<std::mutex> aquireLock(m_mutex);
    //EnterCriticalSection(&m_criticalSection);
    Workload load;
    load.m_job = job;
    m_jobs.push_back(load);
    //LeaveCriticalSection(&m_criticalSection);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
Workload JobQueue::GetNextWorkLoad()
{
    std::scoped_lock<std::mutex> aquireLock(m_mutex);
    Workload workload;
    //Use critical sections for now
    //EnterCriticalSection(&m_criticalSection);
    if (!m_jobs.empty())
    {
        workload = *m_jobs.rbegin();
        m_jobs.pop_back();
    }
    //LeaveCriticalSection(&m_criticalSection);
    return workload;
}

std::atomic<size_t> globalCounter = 0;

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void SimplePrintTask::Execute(size_t threadIndex)
{
    std::stringstream str("");
    str << ++globalCounter << " Thread " << threadIndex << " executing\n";
    OutputDebugString(str.str().c_str());
}
