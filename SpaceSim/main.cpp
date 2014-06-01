#include "BaseApplication.h"
#include <stdio.h>
#include <tchar.h>
#include "Paths.h"
#include "texture.h"
#ifdef _DEBUG
#include "vld.h"
#endif
#include <iostream>
#include <direct.h>

#include "main.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //int sizeOfList = SizeT<node1>::value;
    //sizeOfList = HashT<0, node1>::value;
    //UNUSEDPARAM(sizeOfList);
    UNUSEDPARAM(hInstance);
    UNUSEDPARAM(hPrevInstance);
    UNUSEDPARAM(szCmdLine);
    UNUSEDPARAM(iCmdShow);

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