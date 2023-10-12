#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <source_location>
#include <string>
#include <sstream>

#include <Windows.h>
#include <Stringapiset.h>
#include <vector>

const size_t c_fnvHashOffset = 2166136261;
const size_t c_fnvHashPrime = 16777619;


enum class TraceSeverity
{
    EDEBUG,
    ELOG,
    EWARN,
    EERROR,
    EASSERT
};

//This is first so we can use the macros in debugging the functions below
//void debugOutput(TraceSeverity severity, const std::string& prefix, const char* file, int line, const char* format, ...);
void debugOutput(TraceSeverity severity, const std::string& prefix, const std::source_location& location, const char* format, ...);

//#define MSG_TRACE_WITH_FILE_LINENUMBER(severity, channel, msg, ...) debugOutput(severity, channel, __FILE__, __LINE__, msg, __VA_ARGS__);
#define MSG_TRACE_WITH_FILE_LINENUMBER(severity, channel, msg, ...) debugOutput(severity, channel, std::source_location::current(), msg, __VA_ARGS__);


//#ifndef _DEBUG
//#define MSG_TRACE(message_string, ...) 
//#define MSG_TRACE_CHANNEL(channel, msg, ...) 
//#else
#define MSG_TRACE(message_string, ...) MSG_TRACE_WITH_FILE_LINENUMBER(TraceSeverity::EDEBUG, "", message_string, __VA_ARGS__);
#define MSG_TRACE_CHANNEL(channel, msg, ...) MSG_TRACE_WITH_FILE_LINENUMBER(TraceSeverity::EDEBUG, channel, msg, __VA_ARGS__);

#define MSG_WARN_CHANNEL(channel, msg, ...) MSG_TRACE_WITH_FILE_LINENUMBER(TraceSeverity::EWARN, channel, msg, __VA_ARGS__);
#define MSG_ERROR_CHANNEL(channel, msg, ...) MSG_TRACE_WITH_FILE_LINENUMBER(TraceSeverity::EERROR, channel, msg, __VA_ARGS__);
#define MSG_ASSERT_CHANNEL(channel, msg, ...) MSG_TRACE_WITH_FILE_LINENUMBER(TraceSeverity::EASSERT, channel, msg, __VA_ARGS__);
//#endif

#define HASH_ELEMENT_DEFINITION(CLASS) static constexpr size_t m_hash = #CLASS##_hash;
#define HASH_ELEMENT_TEMPLATE_DEFINITION(CLASS, T) static constexpr size_t m_hash = #CLASS#T##_hash;

std::string FormatString(const char* format, ...);
std::wstring FormatString(const wchar_t* format, ...);

constexpr char toLowerConstExpr(const char c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return c + ('a' - 'A');
    }
    return c;
}

constexpr char toUpperConstExpr(const char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return c + ('a' + 'A');
    }
    return c;
}

///-----------------------------------------------------------------------------
///! @brief   Case insensitive hash function constexpr version
///! @remark  This wont cope with UTF encoded strings
///-----------------------------------------------------------------------------
inline constexpr size_t hashString(const char* sourceStr, size_t size)
{
    size_t returnHash = c_fnvHashOffset;
    for (size_t counter = 0; counter < size; ++counter)
    {
        returnHash = returnHash ^ toLowerConstExpr(sourceStr[counter]);
        returnHash = returnHash * c_fnvHashPrime;
    }

    return returnHash;
}

///-----------------------------------------------------------------------------
///! @brief User defined hash literal, hashes the string with constexpr
///! @remark
///-----------------------------------------------------------------------------
constexpr size_t operator ""_hash(const char* str, size_t size)
{
    return hashString(str, size);
}

/////-----------------------------------------------------------------------------
/////! @brief   Case insensitive hash function, uses the const expression version
/////! @remark  in C++20 this should become a constexpr function, waiting for constexpr containers
/////-----------------------------------------------------------------------------
//inline constexpr size_t hashString(const std::string& sourceStr)
//{
//    return hashString(sourceStr.c_str(), sourceStr.size());
//}

///-----------------------------------------------------------------------------
///! @brief   Case insensitive hash function, uses the const expression version
///! @remark  in C++20 this should become a constexpr function, waiting for constexpr containers
///-----------------------------------------------------------------------------
inline constexpr size_t hashString(const std::string_view& sourceStr)
{
    return hashString(sourceStr.data(), sourceStr.size());
}

inline constexpr size_t hashBinaryData(const char* data, size_t size)
{
    size_t returnHash = c_fnvHashOffset;
    for (size_t counter = 0; counter < size; ++counter)
    {
        returnHash = returnHash ^ data[counter];
        returnHash = returnHash * c_fnvHashPrime;
    }

    return returnHash;
}

#define HASH_STRING(x) hashString(#x)
#define HASH_BINARY(x) hashBinaryData(reinterpret_cast<const char*>(x), sizeof(*x))
///-------------------------------------------------------------------------
// @brief Safe string copy
///-------------------------------------------------------------------------
template <size_t charCount>
void stringCopy(char (&output)[charCount], const char* source)
{
    strncpy_s(output, source, charCount);
    output[charCount -1] = 0;
}

/////-----------------------------------------------------------------------------
/////! @brief   Case sensitive hash function const_expr
/////! @remark
/////-----------------------------------------------------------------------------
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

inline std::string toLowerCase(const std::string_view& str)
{
    std::string result;
    result.resize(str.length());
    std::transform(str.begin(), str.end(), result.begin(), [](char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

inline std::string toUpperCase(const std::string_view& str)
{
    std::string result;
    result.resize(str.length());
    std::transform(str.begin(), str.end(), result.begin(), [](char c) { return static_cast<char>(std::toupper(c)); });
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

///-------------------------------------------------------------------------
// @brief Filename can't contain "." in the name, relative path is fine
///-------------------------------------------------------------------------
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

///-------------------------------------------------------------------------
// @brief Filename can't contain "." in the name, relative path is fine
///-------------------------------------------------------------------------
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
 std::wstring convertToWideString(const std::string str);
 void convertToUTF16String(const std::string& str, std::wstring& out);

void convertToCString(const std::wstring& str, std::string& out);

std::vector<std::string> tokeniseString(const std::string& input, const char delimeter);
std::string trim(const std::string_view& input);