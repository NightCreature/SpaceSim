#include "Core/StringOperations/StringHelperFunctions.h"
#include "Application/BaseApplication.h"

//import TestModules;

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void convertToCString(const std::wstring& str, std::string& out)
{
    char buffer[4096];
    // BOOL usedDefaultChar = FALSE;
    int numberOfCChars = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str.c_str(), (int)str.size(), buffer, 0, 0, 0);
    if (numberOfCChars > 0)
    {
        out.reserve(numberOfCChars);
        WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str.c_str(), (int)str.size(), buffer, numberOfCChars, 0, 0);
        buffer[numberOfCChars] = 0;
        out = buffer;
    }
    else
    {
        DWORD error = GetLastError();
        HRESULT hr = HRESULT_FROM_WIN32(error);
        MSG_TRACE_CHANNEL("String Conversion Error", "Failed to convert from MB to UTF8 with Hresult: 0x%08x, %s", hr, getLastErrorMessage(error));
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
std::vector<std::string> tokeniseString(const std::string& input, const char delimeter)
{
    std::vector<std::string> result;
    if (!input.empty())
    {
        size_t tokenBegin = 0;
        bool foundToken = false;
        for (size_t counter = 0; counter < input.size(); ++counter)
        {
            if (input[counter] == delimeter)
            {
                result.push_back(input.substr(tokenBegin, counter));
                tokenBegin = counter + 1;
                foundToken = true;
            }
        }

        if (!foundToken)
        {
            result.push_back(input);
        }
        else
        {
            //Lets not miss the ending of the string as a token
            if (tokenBegin != input.size()) //Not at end of the string
            {
                result.push_back(input.substr(tokenBegin, input.size()));
            }
        }
    }

    return result;
}

///-----------------------------------------------------------------------------
///! @brief Trims leading white spaces from beginning and end of string
///! @remark
///-----------------------------------------------------------------------------
std::string trim(const std::string_view& input)
{
    constexpr char whiteSpaceDelimiters[] = { '\r\n', '\n', '\r', '\t',' ' };
    bool needsReverseSearch = false;
    size_t leadingIndex = 0;
    size_t trailingIndex = input.length() - 1;

    //Walk the string from both sides
    size_t tempLead = trailingIndex;
    size_t tempTrail = leadingIndex;
    while (trailingIndex != leadingIndex && (leadingIndex != tempLead || trailingIndex != tempTrail))
    {
        bool foundDelimiter = false;
        tempTrail = trailingIndex;
        tempLead = leadingIndex;

        for (const auto& delimiter : whiteSpaceDelimiters)
        {
            if (input[tempLead] == delimiter)
            {
                ++leadingIndex;
            }

            if (input[tempTrail] == delimiter)
            {
                --trailingIndex;
            }
        }
    }

    std::string result = "";
    if (trailingIndex != leadingIndex)
    {
        result = input.substr(leadingIndex, trailingIndex - leadingIndex + 1);
    }
    return result;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void convertToWideString(const std::string& str, std::wstring& out)
{
    wchar_t buffer[4096];
    int numberOfWideChars = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), (int)str.size(), buffer, 0);
    if (numberOfWideChars > 0)
    {
        out.reserve(numberOfWideChars);
        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), (int)str.size(), buffer, numberOfWideChars);
        buffer[numberOfWideChars] = 0;
        out = buffer;
    }
    else
    {
        DWORD error = GetLastError();
        HRESULT hr = HRESULT_FROM_WIN32(error);
        MSG_TRACE_CHANNEL("String Conversion Error", "Failed to convert from UTF8 to MB with Hresult: 0x%08x, %s", hr, getLastErrorMessage(error));
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
std::wstring convertToWideString(const std::string str)
{
    std::wstring output;
    convertToWideString(str, output);
    return output;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void convertToUTF16String(const std::string& str, std::wstring& out)
{
    wchar_t buffer[4096];
    int numberOfWideChars = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), (int)str.size(), buffer, 0);
    if (numberOfWideChars > 0)
    {
        out.reserve(numberOfWideChars);
        MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.c_str(), (int)str.size(), buffer, numberOfWideChars);
        buffer[numberOfWideChars] = 0;
        out = buffer;
    }
    else
    {
        DWORD error = GetLastError();
        HRESULT hr = HRESULT_FROM_WIN32(error);
        MSG_TRACE_CHANNEL("String Conversion Error", "Failed to convert from UTF8 to MB with Hresult: 0x%08x, %s", hr, getLastErrorMessage(error));
    }
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
//void debugOutput(TraceSeverity severity, const std::string& prefix, const char* file, int line, const char * format, ...)
//{
//    char buf[4096];
//    va_list args;
//    va_start(args, format);
//    vsprintf_s(buf, format, args);
//    va_end(args);
//    char debugOutputStr[5120];
//    std::string outputFormatString = "";
//    std::string prefixInternal = prefix;
//    if (prefix.empty())
//    {
//        prefixInternal = "TRACE";
//    }
//    prefixInternal = toUpperCase(prefixInternal);
//    
//    switch (severity)
//    {
//    default:
//    case TraceSeverity::EDEBUG:
//    case TraceSeverity::ELOG:
//        outputFormatString = "%s(%d): [%s]  %s\n";
//        break;
//    case TraceSeverity::EWARN:
//
//        outputFormatString = "%s(%d): [%s] WARNING : %s\n";
//        break;
//    case TraceSeverity::EERROR:
//
//        outputFormatString = "%s(%d): [%s] : ERROR : %s\n";
//        break;
//    case TraceSeverity::EASSERT:
//
//        outputFormatString = "%s(%d): [%s] : ASSERT : %s\n";
//        break;
//    }
//    sprintf_s(debugOutputStr, 5120, outputFormatString.c_str(), file, line, prefixInternal.c_str(), buf);
//    Application::m_logger.LogMessage(debugOutputStr);
//}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void debugOutput(TraceSeverity severity, const std::string& prefix, const std::source_location& location, const char* format, ...)
{
    char buf[4096];
    va_list args;
    va_start(args, format);
    vsprintf_s(buf, format, args);
    va_end(args);
    char debugOutputStr[5120];
    std::string outputFormatString = "";
    std::string prefixInternal;
    if (prefix.empty())
    {
        prefixInternal = "TRACE";
    }
    else
    {
        prefixInternal = toUpperCase(prefix);
    }

    switch (severity)
    {
    default:
    case TraceSeverity::EDEBUG:
    case TraceSeverity::ELOG:
        outputFormatString = "%s(%d): [%s]  %s\n";
        break;
    case TraceSeverity::EWARN:

        outputFormatString = "%s(%d): [%s] WARNING : %s\n";
        break;
    case TraceSeverity::EERROR:

        outputFormatString = "%s(%d): [%s] : ERROR : %s\n";
        break;
    case TraceSeverity::EASSERT:

        outputFormatString = "%s(%d): [%s] : ASSERT : %s\n";
        break;
    }
    sprintf_s(debugOutputStr, 5120, outputFormatString.c_str(), location.file_name(), location.line(), prefixInternal.c_str(), buf);
    Application::m_logger.LogMessage(debugOutputStr);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
std::string FormatString(const char* format, ...)
{
    static char buf[2048];
    va_list args;
    va_start(args, format);
    vsprintf_s(buf, format, args);
    va_end(args);

    return buf;
}


///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
std::wstring FormatString(const wchar_t* format, ...)
{
    static wchar_t buf[2048];
    va_list args;
    va_start(args, format);
    vswprintf_s(buf, format, args);
    va_end(args);

    return buf;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
bool isReletiveFileName(const std::string& file_name)
{
    if (file_name[0] == '.' && file_name[1] == '.')
    {
        return true;
    }

    return false;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
std::string makeAbsolutePath(const std::string& filename)
{
    std::string file_name = filename;
    if (isReletiveFileName(file_name))
    {
        //Get current working directory adn strip the relative stuff
        while (file_name[0] == '.' && file_name[1] == '.')
        {
            file_name.erase(0, 3);
        }
    }
    return file_name;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a 1200
///! @remark
///-----------------------------------------------------------------------------
char* getLastErrorMessage(DWORD nErrorCode)
{
    char* msg = nullptr;
    // Ask Windows to prepare a standard message for a GetLastError() code:
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, nErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msg, 0, NULL);
    // Return the message
    if (!msg)
        return nullptr;// "Unknown error";
    else
        return(msg);
}
