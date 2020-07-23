#pragma once

#include "Threading/Thread.h"

class JobSystem;

class WorkerThread : public Thread
{
public:
    WorkerThread() = default;

    void SetJobsystem(JobSystem* JobSystem, size_t index, HANDLE handle) { m_jobSystem = JobSystem; m_index = index; m_workAvailableHandle = handle; }
    virtual int WorkerFunction() override final;

    virtual void pauzeThread();

    JobSystem* m_jobSystem;
    size_t m_index = 0;
    HANDLE m_workAvailableHandle;

    bool m_threadPaused = false;
};

