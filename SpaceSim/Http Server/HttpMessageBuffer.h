#pragma once

//#include "httpserver.h"
//#include "../SpaceSim/GameResource.h"
//
//#include <string>
//#include <vector>
//
//class MessageTask : public IHttpTask
//{
//public:
//    MessageTask(std::vector<std::string> messages) : m_messages(messages) {}
//
//    virtual void DoTask() override {}
//
//    std::vector<std::string> m_messages;
//};
//
//class HttpMessageBuffer
//{
//public:
//    HttpMessageBuffer(Resource* resource) : m_gameResource(resource) {}
//
//    //-----------------------------------------------------------------------------
//    //! @brief   TODO enter a description
//    //! @remark
//    //-----------------------------------------------------------------------------
//    void AddMessages(const std::vector< std::string >& buffer)
//    {
//        MessageTask* task = new MessageTask(buffer);
//
//        m_gameResource.getWritableGameResource().getHTTPServer().AddTask(task);
//    }
//
//private:
//    GameResourceHelper m_gameResource;
//};