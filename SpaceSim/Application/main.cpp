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

class TestClass
{
private:

    int m_bitfield1 : 4;
    std::string m_string;
    int m_bitfield2 : 4;
    int m_int;
};

int ConvertStringToInt(const char* string)
{
    //assert(string, "Input string is not a valid string") :
    //We need to check whether we have a minus sign in the first part of the string
    char whitespaces[] = { ' ','\t' ,'\n' ,'\v' ,'\f' ,'\r' };
    bool isNegative = false;
    int currentIndex = 0;
    bool isWhitespace = true;
    while (string[currentIndex] != '\0' && isWhitespace)
    {
        isWhitespace = false;
        for (int counter = 0; counter < 6; ++counter)
        {
            if (string[currentIndex] == whitespaces[counter])
            {
                isWhitespace = true;
            }
        }

        if (isWhitespace)
        {
            ++currentIndex;
        }
    }

    if (string[currentIndex] == '-')
    {
        isNegative = true;
        ++currentIndex;
    }

    int value = 0;

    while (string[currentIndex] != '\0')
    {
        int digit = string[currentIndex] - '0';
        if (digit < 0 || digit > 9)
        {
            printf("input string contains a non numerical value!"); //Indicate an error to standard console and return the value found up untill the error note however that the value is incorrect but the program won't crash
            return value;
        }

        value *= 10;
        value += digit;
        ++currentIndex;
    }

    if (isNegative)
        value = -value;

    return value;
}

//class SingleTonTest
//{
//public:
//    static SingleTonTest& getInstance() 
//    {
//        static SingleTonTest instance; 
//        return instance; 
//    }
//
//    void printMember()
//    {
//        std::cout << m_member << std::endl;
//    }
//
//private:
//    SingleTonTest() :m_intAllocator(512), m_linearAllocator(512), m_heapAllocator(512), m_arena(m_intAllocator), m_member(1)
//    {
//        void* floatAllocation = m_linearAllocator.allocate(sizeof(TestClass));
//        void* shortAllocation = m_linearAllocator.allocate(sizeof(short));
//        void* block128Size = m_linearAllocator.allocate(128);
//        void* unAlignedAllocation = m_linearAllocator.allocate(56);
//
//        void* allocateSomething = m_heapAllocator.allocate(sizeof(TestClass));
//
//        m_heapAllocator.deallocate(allocateSomething);
//        allocateSomething = nullptr;
//
//        m_linearAllocator.clear();
//
//        void* intAllocation = m_arena.allocate(sizeof(int), SOURCE_INFO);
//        ALLOCATE_FROM_ARENA(m_arena, sizeof(int));
//    }
//    Allocator::IntPoolAllocator m_intAllocator;
//    Allocator::Block16AlignedAllocator m_linearAllocator;
//    Allocator::Heap16Allocator m_heapAllocator;
//
//    Allocator::MemoryArena m_arena;
//    int m_member;
//};

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
    const char* buffer = "\n-2147483649";
    int x = ConvertStringToInt(buffer);
    x = atoi(buffer);
    x = x;

    //SingleTonTest& instance = SingleTonTest::getInstance();
    //instance.printMember();

//#ifdef _DEBUG
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
//#endif

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