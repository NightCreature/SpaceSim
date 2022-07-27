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

    JobQueue* GetJobQueuePtr() { return &m_jobQueue; }

    void WorkerThreadSleeping(size_t index);
    void WorkerThreadActive(size_t index);

    void SignalWorkAvailable();
    void WaitfForJobsToFinish(); //This doesnt allow you to steal this thread to run a job on it

    void ProcessWork(); //this thread will also be used to execute jobs on

    struct ThreadStatus
    {
        WorkerThread m_thread;
        bool m_working;
    };

    std::vector<ThreadStatus> m_workerThreads;
    JobQueue m_jobQueue;

    HANDLE m_workAvaliable; //Signal event to worker threads that they should wake up
    HANDLE m_workFinishedEvent; //Signal the job system uses to figure out if all work has been done if we want to block

    std::mutex m_finishedMutex;
    std::atomic_size_t m_numberOfSleepingThreads;
};

