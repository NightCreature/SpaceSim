#include "WebkitThread.h"

#include "StringHelperFunctions.h"
#include "TypeHelpers.h"

double WebkitThread::m_elapsedTimeOnThread = 0.0;
std::atomic<int> WebkitThread::m_atomicInt = 0;

WebkitThread::WebkitThread()
{
    m_runThread = true;
}


WebkitThread::~WebkitThread()
{
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
DWORD WebkitThread::ThreadEntryFunction(void* userData)
{
    WebkitThread* thisThread = static_cast<WebkitThread*>(userData);
    while (thisThread->threadActive())
    {
        if (!thisThread->isInitialised())
        {
            if (!thisThread->Initialise())
            {
                return 0;
            }
        }
            
        thisThread->getTimer().update();
        m_elapsedTimeOnThread = static_cast<double>(thisThread->getTimer().getElapsedTime());
    }

    return 1;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool WebkitThread::isInitialised()
{
    return m_initialised;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool WebkitThread::Initialise()
{
#ifdef _DEBUG
    const char* eaWebkitDllName = "EAWebKitd";
#else
    const char* eaWebkitDllName = "EAWebKit";
#endif

    m_eaWebkitDllHandle = LoadLibrary(eaWebkitDllName);
    if (!m_eaWebkitDllHandle)
    {
        MSG_TRACE_CHANNEL("WebkitThread", "Failed to load the webkit dll, &s", eaWebkitDllName);
        return false;
    }

    //Fetch the EAWebkit init function here
    CreateEAWebkitInstance* createLibInstance = reinterpret_cast<CreateEAWebkitInstance*>(GetProcAddress(m_eaWebkitDllHandle, "CreateEAWebkitInstance"));
    if (!createLibInstance)
    {
        MSG_TRACE_CHANNEL("WebkitThread", "Failed to get the address to the lib creation function");
        return false;
    }

    m_eaWebkitLib = createLibInstance();
    if (!m_eaWebkitLib)
    {
        MSG_TRACE_CHANNEL("WebkitThread", "Failed to create the webkit lib instance");
        return false;
    }

    EA::WebKit::AppCallbacks callbacks;
    ZeroMemory(&callbacks, sizeof(callbacks));
    callbacks.timer = &timerCallback;
    callbacks.stackBase = &stackBaseAddress;
    callbacks.atomicIncrement = &atomicIncrement;
    callbacks.atomicDecrement = &atomicDecrement;
    callbacks.cryptRandomValue = &cryptoRandomValue;
    EA::WebKit::AppSystems appSystems;
    ZeroMemory(&appSystems, sizeof(appSystems));
    if (!m_eaWebkitLib->Init(&callbacks, &appSystems))
    {
        MSG_TRACE_CHANNEL("WebkitThread", "Failed to initialise EA Webkit");
        return false;
    }

    m_initialised = true;
    return true;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool WebkitThread::threadActive()
{
    return m_runThread;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
double WebkitThread::timerCallback()
{
    return m_elapsedTimeOnThread;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void* WebkitThread::stackBaseAddress()
{
    // offset 0x18 from the FS segment register gives a pointer to
    // the thread information block for the current thread
    PNT_TIB64 pTib = reinterpret_cast<PNT_TIB64>(NtCurrentTeb());
    return reinterpret_cast<void*>(pTib->StackBase);
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
int WebkitThread::atomicIncrement(int volatile *addend)
{
    return m_atomicInt += *addend;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
int WebkitThread::atomicDecrement(int volatile *addend)
{
    return m_atomicInt -= *addend;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void WebkitThread::cryptoRandomValue(unsigned char *buffer, size_t length)
{
    UNUSEDPARAM(buffer);
    UNUSEDPARAM(length);
}
