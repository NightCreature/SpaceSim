#include "Core/StringOperations/StringHelperFunctions.h"
#include "Application/BaseApplication.h"

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void debugOutput(TraceSeverity severity, const std::string& prefix, const char* file, int line, const char * format, ...)
{
    static char buf[2048];
    va_list args;
    va_start(args, format);
    vsprintf_s(buf, format, args);
    va_end(args);
    static char debugOutputStr[3072];
    std::string outputFormatString = "";
    std::string prefixInternal = prefix;
    if (prefix.empty())
    {
        prefixInternal = "TRACE";
    }
    switch (severity)
    {
    default:
    case EDEBUG:
    case ELOG:
        outputFormatString = "%s(%d): [%s]  %s\n";
        break;
    case EWARN:

        outputFormatString = "%s(%d): [%s] WARNING : %s\n";
        break;
    case EASSERT:

        outputFormatString = "%s(%d): [%s] : ASSERT : %s\n";
        break;
    }
    sprintf_s(debugOutputStr, 3072, outputFormatString.c_str(), file, line, prefixInternal.c_str(), buf);
    Application::m_logger.LogMessage(debugOutputStr);
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool isReletiveFileName(const std::string& file_name)
{
    if (file_name[0] == '.' && file_name[1] == '.')
    {
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//! @brief   TODO enter a 1200
//! @remark
//-----------------------------------------------------------------------------
char* getLastErrorMessage(DWORD nErrorCode)
{
    char* msg = nullptr;
    // Ask Windows to prepare a standard message for a GetLastError() code:
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, nErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msg, 0, NULL);
    // Return the message
    if (!msg)
        return("Unknown error");
    else
        return(msg);
}
