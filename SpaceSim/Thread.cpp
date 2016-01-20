#include "Thread.h"
#include "StringHelperFunctions.h"


Thread::~Thread()
{
    stopThread();

    DeleteCriticalSection(&m_criticalSection);
}

void Thread::createThread(int stacksize)
{
    m_thread = CreateThread(NULL, stacksize, (unsigned long(__stdcall *)(void *))this->run, (void *)this, 0, NULL);
    if (m_thread != 0)
    {
        MSG_TRACE("Thread is created, with stacksize: %d", stacksize);
        m_kill = false;
    }
}

int Thread::run(void* parameter)
{
    Thread* thisthread = reinterpret_cast<Thread*>(parameter);
    if (thisthread)
        return thisthread->workerFunction();
    return 0;
}

void Thread::stopThread()
{
    if (m_thread)
    {
        WaitForSingleObject(m_thread, INFINITE);
        CloseHandle(m_thread);
        m_thread = 0;
    }
    MSG_TRACE("Thread is destroyed");
}