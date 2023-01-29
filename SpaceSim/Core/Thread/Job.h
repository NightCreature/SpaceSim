#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"

#include <mutex>
#include <vector>
#include <windows.h>
#include <atomic>

class Job
{
public:
    Job() :
        m_numberOfUnfinishedJobs(1)
    {
    }
    virtual ~Job() = default;
    
    virtual void Execute(size_t threadIndex) = 0;
    virtual void Finish() = 0;

    void FinishJob()
    {
        --m_numberOfUnfinishedJobs;
        if (m_numberOfUnfinishedJobs < 0)
        {
            Finish(); //Do anything that needs to happen when the job is done

            if (m_parent)
            {
                m_parent->FinishJob();
            }
        }
    }

    bool IsFinished() const { return m_numberOfUnfinishedJobs == 0; }

    std::atomic<size_t> m_numberOfUnfinishedJobs;
    Job* m_parent = nullptr;
};

inline void CreateJobAsChild(Job* parent, Job* currentJob)
{
    ++(parent->m_numberOfUnfinishedJobs);
    currentJob->m_parent = parent;
}

struct Workload
{
    Job* m_job;
    bool m_empty = true;

    bool IsFinished() { return m_job->m_numberOfUnfinishedJobs == 0; }
};


class SimplePrintTask //: public Job
{
public:
    virtual void Execute(size_t threadIndex);
};

template<class Function>
class FunctionWrapperJob : public Job
{
public:
    FunctionWrapperJob(Function func) : m_function(func) {}

    void Execute(size_t threadIndex) override
    {
        m_function. template operator()(threadIndex);
    }

    void Finish() {}
private:

    Function m_function;
};

template <typename T>
concept IsNotAJob = !std::is_base_of<Job, T>::value;

template <typename T>
concept IsAJob = requires
{
    std::is_base_of<Job, T>::value;
};

template<class T>
FunctionWrapperJob<T>* CreateFunctionWrapper(T lambda) { return new FunctionWrapperJob<T>(lambda); }

class JobQueue
{
public:
    JobQueue();

    template <class Function>
    void
    AddJob(Function function)
    {
        MSG_TRACE_CHANNEL("JobQueue", "Adding Function job"); 
        FunctionWrapperJob<Function>* fwj = CreateFunctionWrapper(function);
        AddJob(fwj);
    }


    template <class Function>
    void
    AddChildJob(Function function, Job* parent)
    {
        MSG_TRACE_CHANNEL("JobQueue", "Adding Child Function job");
        FunctionWrapperJob<Function>* fwj = CreateFunctionWrapper(function);
        AddChildJob(fwj, parent);
    }

    template<IsAJob T>
    void AddJob(T job)
    {
        MSG_TRACE_CHANNEL("JobQueue", "Adding a job");
        std::scoped_lock<std::mutex> aquireLock(m_mutex);
        Workload load;
        load.m_empty = false;
        load.m_job = job;
        load.m_job->m_parent = nullptr;

        m_jobs.push_back(load);
    }
    
    template<IsAJob T>
    void AddChildJob(T* job, Job* parent)
    {
        MSG_TRACE_CHANNEL("JobQueue", "Adding a child job");
        std::scoped_lock<std::mutex> aquireLock(m_mutex);
        Workload load;
        load.m_empty = false;
        load.m_job = job;
        load.m_job->m_parent = parent;
        ++load.m_job->m_parent->m_numberOfUnfinishedJobs;

        m_jobs.push_back(load);
    }

    bool IsEmpty() const { return m_jobs.empty(); }

    Workload GetNextWorkLoad();

    std::mutex m_mutex;
    std::vector<Workload> m_jobs;
};