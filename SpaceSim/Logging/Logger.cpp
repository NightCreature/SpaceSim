#include "Logger.h"
//#include "../Http Server/HttpMessageBuffer.h"

#include <Windows.h>
#include <Stringapiset.h>

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void OutputDebugLog::LogMessage(const std::string& message)
{
    OutputDebugStringA(message.c_str());
}

////-----------------------------------------------------------------------------
////! @brief   TODO enter a description
////! @remark
////-----------------------------------------------------------------------------
//void HttpDebugLog::LogMessage(const std::string& message)
//{
//    if (m_buffer.size() == 25)
//    {
//        //send them off to the http console
//        m_httpMessageBuffer->AddMessages(m_buffer);
//        m_buffer.clear();
//    }
//
//    m_buffer.push_back(message);
//}
