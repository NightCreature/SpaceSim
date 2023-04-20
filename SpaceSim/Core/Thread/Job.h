#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/Types/TypeHelpers.h"

#include <mutex>
#include <vector>
#include <windows.h>
#include <atomic>

struct ThreadContext;

class Job
{
public:
    Job() :
        m_numberOfUnfinishedJobs(1)
    {}
    virtual ~Job() = default;
    
    virtual bool Execute(ThreadContext* context) = 0;
    virtual void Finish(ThreadContext* context) = 0; //This function is called before we delete the object, this can be handy if you know the job is a parent and relying on actions done in its children

    void FinishJob()
    {
        --m_numberOfUnfinishedJobs;
        if (m_numberOfUnfinishedJobs <= 0) //Since this can be called form a child it, the parent can go to 0 and destroy its object before we Call finish on the parent
        {
            if (m_parent)
            {
                m_parent->FinishJob();
            }
        }
    }

    bool IsFinished() const { return m_numberOfUnfinishedJobs == 0; }

    std::atomic<size_t> m_numberOfUnfinishedJobs;
    Job* m_parent = nullptr;
    bool m_waitingForChildren = false;
};

struct Workload
{
    Job* m_job;
    bool m_empty = true;

    bool IsFinished() { return m_job->m_numberOfUnfinishedJobs == 0; }
};


class SimplePrintTask //: public Job
{
public:
    virtual void Execute(size_t threadIndex, ThreadContext* context);
};

template<class Function>
class FunctionWrapperJob : public Job
{
public:
    explicit FunctionWrapperJob(Function func) : m_function(func) {}

    bool Execute(ThreadContext* context) override
    {
        return m_function. template operator()(context);
    }

    void Finish(ThreadContext* context) { UNUSEDPARAM(context); }
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
    AddFunctionJob(Function function)
    {
        FunctionWrapperJob<Function>* fwj = CreateFunctionWrapper(function);
        AddJob(fwj);
    }


    template <class Function>
    void
    AddChildFunctionJob(Function function, Job* parent)
    {
        FunctionWrapperJob<Function>* fwj = CreateFunctionWrapper(function);
        AddChildJob(fwj, parent);
    }

    template<IsAJob T>
    void AddJob(T job)
    {
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