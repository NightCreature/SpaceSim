#include "Job.h"

#include <atomic>
#include <sstream>
#include "Core/Types/TypeHelpers.h"


///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
JobQueue::JobQueue()
{
}

///-----------------------------------------------------------------------------
///! @brief We need to make this smarter so it doesnt lose the jobs its not the workerthreads job to delete the job at the end the queue owns them?
///! @remark
///-----------------------------------------------------------------------------
Workload JobQueue::GetNextWorkLoad()
{
    std::scoped_lock<std::mutex> aquireLock(m_mutex);
    Workload workload;
    if (!m_jobs.empty())
    {
        workload = *m_jobs.rbegin();
        m_jobs.pop_back();
    }
    return workload;
}

std::atomic<size_t> globalCounter = 0;

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void SimplePrintTask::Execute(size_t threadIndex, ThreadContext* context)
{
    UNUSEDPARAM(context);
    std::stringstream str("");
    str << ++globalCounter << " Thread " << threadIndex << " executing\n";
    OutputDebugString(str.str().c_str());
}