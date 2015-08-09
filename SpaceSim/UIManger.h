#pragma once

#include "WebkitThread.h"

#include <windows.h>

class UIManger
{
public:
    UIManger() {}
    ~UIManger() {}

    //void initialise();
private:
    //This will need a thread handle so EAWebkit can run on it
    HANDLE m_threadHandle;
    //WebkitThread m_webKitThread;
};

