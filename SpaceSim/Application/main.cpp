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
#include <Optick.h>
#include "Gameplay/ECS/Component.h"
#include "Gameplay/ECS/Components/Components.h"
#include "Core/Hashing/SipHash.h"

#include "Core/Profiler/ProfilerMacros.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{

//#ifdef _DEBUG
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
//#endif

    //SpatialHashMap<int> map;
    //map.Initialise(100, Vector2i64(-100), Vector2i64(100));


    //const uint64 hashValue1 = SipHash::hashString("Hello World!!!", 14);
    //const uint64 hashValue2 = SipHash::hashString("Hello World!!!!", 15); //same as above probably shouldn't be

    //const uint64 hashValue3 = SipHash::hashString("1234567890asdfe", 15);

    //ECS::RegisterComponents();

    UNUSEDPARAM(hInstance);
    UNUSEDPARAM(hPrevInstance);
    UNUSEDPARAM(szCmdLine);
    UNUSEDPARAM(iCmdShow);



    MicroProfileOnThreadCreate("Main");

    //HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    //if (FAILED(hr))
    //{
    //    MSG_TRACE_CHANNEL("COM ERROR", "Failed to make COM act in a multithreaded fashion");
    //}

    OPTICK_START_CAPTURE();
    //turn on profiling

    Application application = Application();
    application.SetCommandLine(szCmdLine);
    auto applicationPath = application.GetPaths().getPath();
    MicroProfileSetEnableAllGroups(true);
    MicroProfileSetForceMetaCounters(true);
    if( application.initialise() ) //Need to take the path manager from shader engine
    {
        size_t hash = "default"_hash;
        MSG_TRACE("default hased is: %x", hash);

        printf("open localhost:1338 in chrome to capture profile data\n");
        printf("press ctrl-c to quit\n");

        MSG_TRACE_CHANNEL_FMT("Main", "commandline is: {}", szCmdLine);

        application.mainGameLoop();

        application.cleanup();
    }
    else
    {
        MSG_TRACE_CHANNEL("", "Press any key to terminate the application followed by an enter" )
        char c;
        std::cin >> c;
    }


    OPTICK_STOP_CAPTURE();
    applicationPath = applicationPath / "Profile";
    OPTICK_SAVE_CAPTURE(applicationPath.string().c_str());
    //OPTICK_SAVE_CAPTURE("D:\\SDK\\Demo\\SpaceSim\\bin\\Profile");
    //OPTICK_SAVE_CAPTURE();
    MicroProfileShutdown();

    return 0;
}