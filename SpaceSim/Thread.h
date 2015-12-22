#pragma once


#pragma once
#include "windows.h"

//Implement termination of the thread in the destructor
class Thread
{
public:
    Thread() { m_kill = false; }
    virtual ~Thread();

    //Thread management
    void createThread(int stacksize);
    void killThread() { m_kill = true; }
    void stopThread();
    void pauzeThread();
    bool isAlive() { return !m_kill; }

    static int __stdcall run(void* parameter);

    //Override this method in the derived class to do the work
    virtual int workerFunction() = 0;// {return 0;}
protected:
private:
    HANDLE m_thread;
    bool m_kill;
};