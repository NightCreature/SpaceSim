#include "WorkerThread.h"

#include "JobSystem.h"
#include <Optick.h>

void ReturnJobToQueueToWait(Workload& workLoad, JobQueue& queue)
{
    //workLoad.m_job->m_waitingForChildren = true;
    queue.AddJob(workLoad.m_job);
    workLoad.m_job = nullptr;
    workLoad.m_empty = true;
}

void CleanupWorkLoad(Workload& workLoad, ThreadContext* context)
{
    if (workLoad.m_job != nullptr)
    {
        workLoad.m_job->Finish(context); //Do operations before we delete the object

        delete workLoad.m_job;
    }
}

///-----------------------------------------------------------------------------
///! @brief Main function of this thread
///! @remark
///-----------------------------------------------------------------------------
int WorkerThread::WorkerFunction()
{
    OPTICK_THREAD("WorkerThread");

    CoInitialize(NULL);
    ThreadContext* context = m_jobSystem->GetThreadStatus(m_index);

    while(isAlive())
    {
        if (m_threadPaused) //if this is set we need to see if there is more work instead of just activating it it will keep toggling other wise instead of waiting
        {
            m_threadPaused = false;
            m_jobSystem->WorkerThreadActive(m_index);
        }
        auto& queue = m_jobSystem->GetJobQueue();
        

        auto workLoad = queue.GetNextWorkLoad();
        if (!workLoad.m_empty)
        {
            OPTICK_EVENT();
            if (!workLoad.IsFinished() && !workLoad.m_job->m_waitingForChildren)
            {//Execute this workload
                if (workLoad.m_job->Execute(context))
                {
                    workLoad.m_job->FinishJob();
                }
                //Job might not be finished here we need to deal with that
                if (!workLoad.m_job->IsFinished())
                {
                    //Other threads might be sleeping
                    if (!queue.IsEmpty())
                    {
                        m_jobSystem->SignalWorkAvailable();
                    }

                    //return the job to the queue lets handle all other first, likely we are done after, we have to mark this somehow too
                    ReturnJobToQueueToWait(workLoad, queue);

                }
            }
            else
            {
                //This job was rescheduled because it children werent finished lets check if they are
                if (!workLoad.m_job->IsFinished())
                {
                    ReturnJobToQueueToWait(workLoad, queue);
                }
            }
            
            //Should deallocate the job here
            CleanupWorkLoad(workLoad, context);
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
    if (isAlive())
    {
        m_threadPaused = true;
        //Probably need to sleep on a condition variable here that the jobsystem can set
        //WaitForSingleObject(m_workAvailableHandle, INFINITE);

        WaitForSingleObject(m_workAvailableHandle, INFINITE);    // indefinite wait
    }

}