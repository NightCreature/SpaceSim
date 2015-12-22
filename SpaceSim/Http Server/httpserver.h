#pragma once

#include <winsock2.h>
#include <http.h>

#include "..\Thread.h"

#include <string>
#include <vector>
//
// Macros.
//
#define INITIALIZE_HTTP_RESPONSE( resp, status, reason )    \
    do                                                      \
    {                                                       \
        RtlZeroMemory( (resp), sizeof(*(resp)) );           \
        (resp)->StatusCode = (status);                      \
        (resp)->pReason = (reason);                         \
        (resp)->ReasonLength = (USHORT) strlen(reason);     \
    } while (FALSE)

#define ADD_KNOWN_HEADER(Response, HeaderId, RawValue)               \
    do                                                               \
    {                                                                \
        (Response).Headers.KnownHeaders[(HeaderId)].pRawValue =      \
                                                          (RawValue);\
        (Response).Headers.KnownHeaders[(HeaderId)].RawValueLength = \
            (USHORT) strlen(RawValue);                               \
    } while(FALSE)

#define ALLOC_MEM(cb) HeapAlloc(GetProcessHeap(), 0, (cb))

#define FREE_MEM(ptr) HeapFree(GetProcessHeap(), 0, (ptr))


class HTTPServer : public Thread
{
public:
    HTTPServer() {}
    ~HTTPServer() {}

    void Initialise();
    void Cleanup();

    void Update();
private:
    unsigned int DoReceiveRequests(HANDLE hReqQueue);
    unsigned int SendHttpResponse(HANDLE hReqQueue, PHTTP_REQUEST pRequest, unsigned short StatusCode, const char* pReason, const char* pEntity );
    unsigned int SendHttpPostResponse( HANDLE hReqQueue, PHTTP_REQUEST pRequest );

    //-----------------------------------------------------------------------------
    //! @brief   TODO enter a description
    //! @remark
    //-----------------------------------------------------------------------------
    virtual int workerFunction() override;

    std::vector< std::wstring > m_urlsToListenTo;
    HANDLE          m_httpRequestQueue;
    HTTPAPI_VERSION m_httpApiVersion;
};