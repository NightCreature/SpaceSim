#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <sstream>

#include <Windows.h>
#include <Stringapiset.h>

#include "Core/Profiler/ProfilerMacros.h"

const unsigned int c_fnvHashOffset = 2166136261;
const unsigned int c_fnvHashPrime = 16777619;

enum TraceSeverity
{
    EDEBUG,
    ELOG,
    EWARN,
    EASSERT
};

//This is first so we can use the macros in debugging the functions below
void debugOutput(TraceSeverity severity, const std::string& prefix, const char* file, int line, const char * format, ...);

#define MSG_TRACE_WITH_FILE_LINENUMBER(severity, channel, msg, ...) debugOutput(severity, channel, __FILE__, __LINE__, msg, __VA_ARGS__);


//#ifndef _DEBUG
//#define MSG_TRACE(message_string, ...) 
//#define MSG_TRACE_CHANNEL(channel, msg, ...) 
//#else
#define MSG_TRACE(message_string, ...) MSG_TRACE_WITH_FILE_LINENUMBER(EDEBUG, "", message_string, __VA_ARGS__);
#define MSG_TRACE_CHANNEL(channel, msg, ...) MSG_TRACE_WITH_FILE_LINENUMBER(EDEBUG, channel, msg, __VA_ARGS__);
//#endif

#define HASH_ELEMENT_DEFINITION static const unsigned int m_hash;
#define HASH_ELEMENT_IMPLEMENTATION(CLASS) const unsigned int CLASS::m_hash = hashString( #CLASS );

std::string FormatString(const char* format, ...);

//-----------------------------------------------------------------------------
//! @brief   Case insensitive hash function
//! @remark
//-----------------------------------------------------------------------------
inline unsigned int hashString(const std::string& sourceStr)
{
    PROFILE_EVENT("HashString", Brown);
    unsigned int returnHash = c_fnvHashOffset;
    for(unsigned int counter = 0; counter < sourceStr.size(); ++ counter)
    {
        returnHash = returnHash ^ tolower(sourceStr[counter]);
        returnHash = returnHash * c_fnvHashPrime;
    }

    return returnHash;
}

inline size_t hashBinaryData(const char* data, size_t size)
{
    PROFILE_EVENT("hashBinaryData", Brown);
    unsigned int returnHash = c_fnvHashOffset;
    for (unsigned int counter = 0; counter < size; ++counter)
    {
        returnHash = returnHash ^ data[counter];
        returnHash = returnHash * c_fnvHashPrime;
    }

    return returnHash;
}

#define HASH_STRING(x) hashString(#x)
#define HASH_BINARY(x) hashBinaryData(reinterpret_cast<const char*>(x), sizeof(*x))
//-------------------------------------------------------------------------
// @brief Safe string copy
//-------------------------------------------------------------------------
template <size_t charCount>
void stringCopy(char (&output)[charCount], const char* source)
{
    strncpy_s(output, source, charCount);
    output[charCount -1] = 0;
}

////-----------------------------------------------------------------------------
////! @brief   Case sensitive hash function const_expr
////! @remark
////-----------------------------------------------------------------------------
//unsigned int constexpr compileHashString(const char* sourceStr, unsigned int hash_value = c_fnvHashOffset)
//{
//    return sourceStr != nullptr ? compileHashString( sourceStr + 1, (hash_value ^ static_cast<unsigned int>(*sourceStr)) * c_fnvHashPrime) : hash_value;
//}

template <class T>
inline void strToTemplateType(T& convertedValue, 
                 const std::string& str, 
                 std::ios_base& (*inputBaseType)(std::ios_base&))
{
    std::istringstream inputStrStream(str);
    inputStrStream >> inputBaseType >> convertedValue;
}

inline int strToint(const std::string& str, bool hex = false)
{
    int retValue = 0;

    if (hex)
    {
        strToTemplateType<int>(retValue, str, std::hex);
    }
    else
    {
        strToTemplateType<int>(retValue, str, std::dec);
    }

    return retValue;
}

inline double strToDouble(const std::string& str, bool hex = false)
{
    double retValue = 0.0f;
    
    if (hex)
    {
        strToTemplateType<double>(retValue, str, std::hex);
    }
    else
    {
        strToTemplateType<double>(retValue, str, std::dec);
    }

    return retValue;

}

inline float strTofloat(const std::string& str, bool hex = false)
{
    return (float)strToDouble(str, hex);
}

inline std::string toLowerCase(const std::string& str)
{
    std::string result = str;
    std::transform(str.begin(), str.end(), result.begin(), [](char c) { return static_cast<char>(::tolower(c)); });
    return result;
}

inline bool strICmp(const std::string& lhs, const std::string& rhs)
{
    return 0 == strcmp(toLowerCase(lhs).c_str(), toLowerCase(rhs).c_str()); //Avoid nasty 4800 warning by not casting :)
}

inline std::string getTextureNameFromFileName(const std::string& fileName)
{
    const char* startOfFileName = fileName.c_str();
    for (size_t counter = fileName.size(); counter > 0; --counter)
    {
        if (startOfFileName[counter] == '\\' || startOfFileName[counter] == '/')
        {
            startOfFileName = &startOfFileName[counter + 1];
            break; //Break out of the loop as we found the actual filename
        }
    }

    return std::string(startOfFileName);
}

inline std::string getResourceNameFromFileName(const std::string& fileName)
{
    const char* startOfFileName = fileName.c_str();
    for (size_t counter = fileName.size(); counter > 0; --counter)
    {
        if (startOfFileName[counter] == '\\' || startOfFileName[counter] == '/')
        {
            startOfFileName = &startOfFileName[counter + 1];
            break; //Break out of the loop as we found the actual filename
        }
    }

    return std::string(startOfFileName);
}

//-------------------------------------------------------------------------
// @brief Filename can't contain "." in the name, relative path is fine
//-------------------------------------------------------------------------
inline std::string extractExtensionFromFileName( const std::string &fileName )
{
    std::string::const_reverse_iterator it = fileName.rbegin();
    size_t counter = fileName.size();
    for (; counter > 0; --counter)
    {
        if (fileName[counter] == '.')
        {
            break;
        }
    }

    return fileName.substr(counter + 1, fileName.size());
}

//-------------------------------------------------------------------------
// @brief Filename can't contain "." in the name, relative path is fine
//-------------------------------------------------------------------------
inline std::string extractPathFromFileName( const std::string &fileName )
{
    std::string::const_reverse_iterator it = fileName.rbegin();
    size_t counter = fileName.size();
    for (; counter > 0; --counter)
    {
        if (fileName[counter] == '\\' || fileName[counter] == '/')
        {
            break;
        }
    }

    return fileName.substr(0, counter + 1);
}

std::string makeAbsolutePath(const std::string& filename);


 char* getLastErrorMessage(DWORD nErrorCode);

 void convertToWideString(const std::string& str, std::wstring& out);
 void convertToUTF16String(const std::string& str, std::wstring& out);

void convertToCString(const std::wstring& str, std::string& out);