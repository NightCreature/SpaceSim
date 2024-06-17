#pragma once

#include "Core/StringOperations/HashString.h"

#include <string>

namespace MessageSystem
{

    class MessageType
    {
    public:
        MessageType() {}
        virtual ~MessageType() {}

        virtual const std::string_view& GetName() const = 0;
        virtual const HashString& GetId() const = 0;
        virtual size_t GetSize() const = 0;
    protected:
    };

    template<class Component>
    class DefaultMessageType : public MessageType
    {
    public:
        DefaultMessageType(const std::string_view& id, const ComponentTag& tag) : m_id(id), m_tag(tag)
        {
            m_instance = this;
        }

        const std::string_view& GetId() const override { return m_name; }
        const HashString& GetComponentType() const override { return m_messageId; }
        size_t GetSize() const override { return sizeof(Component); }
        //CreateComponentFP GetCreateFP() const override { return &Component::Create; }

        static DefaultMessageType* m_instance;
    private:
        std::string_view m_name;
        HashString m_messageId;
    };

    template<class Message>
    DefaultMessageType<Message>* DefaultMessageType<Message>::m_instance;

}
