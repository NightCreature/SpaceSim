#pragma once

#include "Job.h"
#include "WorkerThread.h"

#include <atomic>
#include <mutex>
#include <vector>

class JobSystem
{
public:
    JobSystem(size_t numThreads);
    ~JobSystem();

    const JobQueue& GetJobQueue() const { return m_jobQueue; }
    JobQueue& GetJobQueue() { return m_jobQueue; }

    void WorkerThreadSleeping(size_t index);
    void WorkerThreadActive(size_t index);

    void SignalWorkAvailable();
    void WaitfForJobsToFinish();

    struct ThreadStatus
    {
        WorkerThread m_thread;
        bool m_working;
    };

    std::vector<ThreadStatus> m_workerThreads;
    JobQueue m_jobQueue;

    HANDLE m_eventHandle;
    HANDLE m_workFinishedEvent;

    std::mutex m_finishedMutex;
    std::atomic_size_t m_numberOfSleepingThreads;
};

