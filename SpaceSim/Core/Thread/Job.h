#pragma once

#include <mutex>
#include <vector>
#include <windows.h>

class Job
{
public:
    Job() = default;
    virtual ~Job() = default;

    virtual void Execute(size_t threadIndex) = 0;
};

struct Workload
{
    Job* m_job = nullptr;
};


class SimplePrintTask : public Job
{
public:
    virtual void Execute(size_t threadIndex);
};

class JobQueue
{
public:
    JobQueue();

    void AddJob(Job* job);

    Workload GetNextWorkLoad();

    CRITICAL_SECTION m_criticalSection;
    std::mutex m_mutex;
    std::vector<Workload> m_jobs;
};