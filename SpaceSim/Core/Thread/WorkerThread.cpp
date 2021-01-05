#include "WorkerThread.h"

#include "JobSystem.h"

///-----------------------------------------------------------------------------
///! @brief Main function of this thread
///! @remark
///-----------------------------------------------------------------------------
int WorkerThread::WorkerFunction()
{
    while(isAlive())
    {
        if (m_threadPaused) //if this is set we need to see if there is more work instead of just activating it it will keep toggling other wise instead of waiting
        {
            m_threadPaused = false;
            m_jobSystem->WorkerThreadActive(m_index);
        }
        auto& queue = m_jobSystem->GetJobQueue();
        auto workLoad = queue.GetNextWorkLoad();
        if (workLoad.m_job != nullptr)
        {
            //Execute this workload

            workLoad.m_job->Execute(m_index);

            //once we are done with the work load get ride of it, this should probably be done better
            delete workLoad.m_job;
            workLoad.m_job = nullptr;
        }
        else
        {
            m_jobSystem->WorkerThreadSleeping(m_index);
            pauzeThread();
        }
    }

    return 0;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void WorkerThread::pauzeThread()
{
    m_threadPaused = true;
    //Probably need to sleep on a condition variable here that the jobsystem can set
    //WaitForSingleObject(m_workAvailableHandle, INFINITE);

    DWORD dwWaitResult = WaitForSingleObject(
        m_workAvailableHandle, // event handle
        INFINITE);    // indefinite wait

}
