#pragma once

#include "Timer.h"

#include <EaWebkit/EaWebkit.h>

#include <atomic>
#include <windows.h>

class WebkitThread
{
public:
    WebkitThread();
    ~WebkitThread();

    static DWORD ThreadEntryFunction(void* userData);

    bool isInitialised();
    bool Initialise();
private:
    typedef EA::WebKit::EAWebKitLib* CreateEAWebkitInstance(void);
    bool threadActive();
    PerformanceTimer& getTimer() { return m_timer; }

    static double timerCallback();
    static void* stackBaseAddress();
    static int atomicIncrement(int volatile *addend);
    static int atomicDecrement(int volatile *addend);
    static void cryptoRandomValue(unsigned char *buffer, size_t length);

    static double m_elapsedTimeOnThread;
    static std::atomic<int> m_atomicInt;

    HMODULE m_eaWebkitDllHandle;
    EA::WebKit::EAWebKitLib* m_eaWebkitLib;
    PerformanceTimer m_timer;
    bool m_runThread;
    bool m_initialised;

};

