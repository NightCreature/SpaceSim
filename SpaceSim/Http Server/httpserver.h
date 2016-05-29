#pragma once
//
//#include <winsock2.h>
//#include <http.h>
//
#include "..\Thread.h"
//
//#include <string>
//#include <vector>
//
//
//
//class IHttpTask
//{
//public:
//    IHttpTask() {}
//    ~IHttpTask() {}
//
//    virtual void DoTask() = 0;
//};
//
class HTTPServer : public Thread
{
public:
    HTTPServer() {}
    ~HTTPServer() {}
//
    void Initialise() {}
    void Cleanup() {}
//
    void Update() {}
//
//    void AddTask(IHttpTask* task) { EnterCriticalSection(&m_criticalSection); m_tasks.push_back(task); LeaveCriticalSection(&m_criticalSection); }
//private:
//    unsigned int DoReceiveRequests(HANDLE hReqQueue);
//    unsigned int SendHttpResponse(HANDLE hReqQueue, PHTTP_REQUEST pRequest, unsigned short StatusCode, const char* pReason, const char* pEntity );
//    unsigned int SendHttpPostResponse( HANDLE hReqQueue, PHTTP_REQUEST pRequest );
//
    //-----------------------------------------------------------------------------
    //! @brief   TODO enter a description
    //! @remark
    //-----------------------------------------------------------------------------
    virtual int workerFunction() override { return 0; }
//
//    std::vector< std::wstring > m_urlsToListenTo;
//    std::vector< IHttpTask* > m_tasks;
//    HANDLE          m_httpRequestQueue;
//    HTTPAPI_VERSION m_httpApiVersion;
};