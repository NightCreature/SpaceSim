#include "StringHelperFunctions.h"




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
        MSG_TRACE_CHANNEL("String Conversion Error", "Failed to convert from MB to UTF8 with Hresult: 0x%08x, %s", hr, iGetLastErrorText(error));
    }
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void convertToWideString(const std::string& str, std::wstring& out)
{
    wchar_t* buffer = &(out[0]);
    int numberOfWideChars = MultiByteToWideChar(CP_UTF8, WC_ERR_INVALID_CHARS, str.c_str(), (int)str.size(), buffer, 0);
    if (numberOfWideChars > 0)
    {
        out.reserve(numberOfWideChars);
        MultiByteToWideChar(CP_UTF8, WC_ERR_INVALID_CHARS, str.c_str(), (int)str.size(), buffer, numberOfWideChars);
    }
    else
    {
        DWORD error = GetLastError();
        HRESULT hr = HRESULT_FROM_WIN32(error);
        MSG_TRACE_CHANNEL("String Conversion Error", "Failed to convert from UTF8 to MB with Hresult: 0x%08x, %s", hr, iGetLastErrorText(error));
    }
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
char* iGetLastErrorText(DWORD nErrorCode)
{
    char* msg;
    // Ask Windows to prepare a standard message for a GetLastError() code:
    FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, nErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&msg, 0, NULL);
    // Return the message
    if (!msg)
        return("Unknown error");
    else
        return(msg);
}
