#include "Memory.h"
#include "BaseApplication.h"
#include <stdio.h>
#include <tchar.h>
#include "Paths.h"
#include "texture.h"

#include <iostream>
#include <direct.h>

#include "main.h"

#include "StringOperations/StringTemplate.h"


class SingleTonTest
{
public:
    static SingleTonTest& getInstance() 
    {
        static SingleTonTest instance; 
        return instance; 
    }

    void printMember()
    {
        std::cout << m_member << std::endl;
    }

private:
    SingleTonTest() : m_member(1) {}
    int m_member;
};

unsigned int constantHash(const char* str, size_t index, unsigned int previousHash)
{
	return str[index] != '\0' ? constantHash(str, index + 1, (previousHash ^ tolower(*str)) * c_fnvHashPrime) : previousHash;
}
unsigned int hashString1(const char* str)
{
	return constantHash(str, 0, c_fnvHashOffset);
}

unsigned int hashString2(const std::string& sourceStr)
{
	return hashString1(sourceStr.c_str());
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{

    SingleTonTest& instance = SingleTonTest::getInstance();
    instance.printMember();

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif
	const char* str = "helloworld";
	std::string str1 = str;
	const unsigned int constHash = hashString1(str);
	unsigned int hashValue = hashString2(str1);
	if (constHash == hashValue)
	{
		MSG_TRACE("Yay Constant expression hashes work :)");
	}
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