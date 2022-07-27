#include "Memory.h"
#include "BaseApplication.h"
#include <stdio.h>
#include <tchar.h>
#include "Core/Paths.h"
#include "Graphics/texture.h"

#include <iostream>
#include <direct.h>

#include "Core/StringOperations/StringTemplate.h"

#include "Memory/SlotAllocator.h"
#include "Memory/LinearAllocator.h"
#include "Memory/HeapAllocator.h"

#include "Memory/MemoryArena.h"

#include "Physics/spatialhashmap.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
//#ifdef _DEBUG
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
//#endif

    SpatialHashMap<int> map;
    map.Initialise(100, Vector2i64(-100), Vector2i64(100));

    UNUSEDPARAM(hInstance);
    UNUSEDPARAM(hPrevInstance);
    UNUSEDPARAM(szCmdLine);
    UNUSEDPARAM(iCmdShow);

    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("COM ERROR", "Failed to make COM act in a multithreaded fashion");
    }

    Application application = Application();
    if( application.initialise() ) //Need to take the path manager from shader engine
    {
        application.mainGameLoop();

        application.cleanup();
    }
    else
    {
        MSG_TRACE_CHANNEL("", "Press any key to terminate the application followed by an enter" )
        char c;
        std::cin >> c;
    }

    return 0;
}