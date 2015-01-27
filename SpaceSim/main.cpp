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

#include "StringOperations/StringTemplate.h"


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //int sizeOfList = SizeT<node1>::value;
    //sizeOfList = HashT<0, node1>::value;
    //UNUSEDPARAM(sizeOfList);
    UNUSEDPARAM(hInstance);
    UNUSEDPARAM(hPrevInstance);
    UNUSEDPARAM(szCmdLine);
    UNUSEDPARAM(iCmdShow);

    String16 test3("LongStringIsTOOLONG TO FIT");
    test3.assign(12, 'h');
    MSG_TRACE("%c %c %c, %s", test3[11], test3.back(), test3.front(), test3.c_str());
    String16 tokens[16];
    char delimters[] = { '1' };
    test3.split(tokens, delimters);
    String16::iterator it1 = test3.begin();
    String16::iterator it2 = test3.end();
    if (it1 == it2)
    {
        MSG_TRACE("iterators are equal");
    }
    else
    {
        MSG_TRACE("iterators are not equal"); 
    }
    for (String16::const_iterator it16 = test3.cbegin(); it16 != test3.cend(); ++it16)
    {
        MSG_TRACE("%c", *it16);
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