#include "Core/Thread/Thread.h"

#include "Core/Types/TypeHelpers.h"
#include "../Profiler/ProfilerMacros.h"

class BaseThreadContext 
{
public:
    BaseThreadContext(const std::string& name, Thread* thread) : m_threadName(name), m_thread(thread) {}
    std::string m_threadName = "";
    Thread* m_thread = nullptr;
};

Thread::~Thread()
{
    stopThread();

    DeleteCriticalSection(&m_criticalSection);
    CoUninitialize();
}

void Thread::createThread(int stacksize, const std::string& name)
{
    BaseThreadContext* context = new BaseThreadContext(name, this);
#ifdef _DEBUG
    m_threadName = name;
#else
    UNUSEDPARAM(name);
#endif
    
    m_thread = CreateThread(NULL, stacksize, (unsigned long(__stdcall *)(void *))this->run, (void *)context, 0, nullptr);
    if (m_thread != 0)
    {
#ifdef _DEBUG
        //MSG_TRACE("Thread(%s) is created, with stacksize: %d", m_threadName.c_str(), stacksize);
#endif
        m_kill = false;
    }
}

int Thread::run(void* parameter)
{
    BaseThreadContext* context = reinterpret_cast<BaseThreadContext*>(parameter);

    PROFILE_THREAD(context->m_threadName.c_str());
    Thread* thisthread = context->m_thread;
    delete context;
    if (thisthread)
        return thisthread->WorkerFunction();
    return 0;
}

void Thread::stopThread()
{
    if (m_thread)
    {
        m_kill = true;
        WaitForSingleObject(m_thread, INFINITE);
        CloseHandle(m_thread);
        m_thread = 0;
       
    }
    //MSG_TRACE("Thread is destroyed");
}