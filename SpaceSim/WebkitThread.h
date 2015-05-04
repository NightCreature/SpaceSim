#pragma once


#include "Timer.h"
#include "ViewManager.h"

#pragma warning( push )
#pragma warning( disable: 4100 )
#include <EAWebKit/EAWebKit>
#pragma warning( pop )

#include <atomic>
#include <deque>
#include <windows.h>

struct CommandData
{

};

class ICommand
{
public:
    ICommand(CommandData* commandData) : m_commandData(commandData) {}
    virtual ~ICommand() {}

    virtual bool RunCommand() = 0;

private:
    CommandData* m_commandData;
};

class WebkitThread
{
public:
    WebkitThread();
    ~WebkitThread();

    static DWORD ThreadEntryFunction(void* userData);

    bool isInitialised();
    bool Initialise();

    enum Command
    {
        CommandLoadUrl,

        //Add new commands above this line
        CommandNumberOfCommands
    };

    void AddCommand(Command command, const CommandData& data);
private:
    typedef EA::WebKit::EAWebKitLib* CreateEAWebkitInstance(void);
    bool threadActive();
    PerformanceTimer& getTimer() { return m_timer; }

    static double timerCallback();
    static void* stackBaseAddress();
    static int atomicIncrement(int volatile *addend);
    static int atomicDecrement(int volatile *addend);
    static void cryptoRandomValue(unsigned char *buffer, size_t length);
    //-----------------------------------------------------------------------------
    //! @brief   TODO enter a description
    //! @remark
    //-----------------------------------------------------------------------------
    void doLoopIteration();
    static double m_elapsedTimeOnThread;
    static std::atomic<int> m_atomicInt;

    HMODULE m_eaWebkitDllHandle;
    EA::WebKit::EAWebKitLib* m_eaWebkitLib;
    PerformanceTimer m_timer;
    std::deque<Command> m_commandQue;
    CRITICAL_SECTION m_criticalSecition;
    ViewManager* m_viewManager;
    bool m_runThread;
    bool m_initialised;

};

