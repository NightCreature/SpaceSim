#pragma once

#include "Core/MessageSystem/Messages.h"
#include "Core/StringOperations/HashString.h"
#include "Core/StringOperations/StringHelperFunctions.h"

#include <array>
#include <string>

namespace MessageSystem
{
class FEMessage : public Message
{
};

namespace FE
{
    class FEMessageData : public MessageData
    {
    public:
        FEMessageData() = default;
        ~FEMessageData() = default;

    private:
        //HashString m_
    };

    template<size_t N>
    class MultiHashStringData : public FEMessageData
    {
    public:
        MultiHashStringData() {}
        MultiHashStringData(const std::array<HashString, N>& data) : m_data(data) {}

        const std::array<HashString, N>& GetData() const { return m_data; }
        void SetData(const std::array<HashString, N>& data) { m_data = data; }

        void SetData(const HashString& data, size_t index)
        {
            static_assert(index < N, "Index is bigger than expected");
            m_data[index] = data;
        }

        const HashString& GetData(size_t index)
        {
            static_assert(index < N, "Index is bigger than expected");
            return m_data[index];
        }
    private:
        std::array<HashString, N> m_data;
    };

    class SelectObjectMessage : public FEMessage
    {
    private:
        static constexpr size_t nrHashes = 2;
        using DoubleHashSringData = MultiHashStringData<nrHashes>;
    public:
        SelectObjectMessage()
        {
            m_MessageId = "SelectObjectMessage"_hash;
            m_implementationData = new DoubleHashSringData();
            m_implementationDataSize = sizeof(DoubleHashSringData);
        }

        ~SelectObjectMessage() {}

        void SetObject(const std::array<HashString, nrHashes>& object)
        {
            if (m_implementationData != nullptr)
            {
                auto* hashStringData = static_cast<DoubleHashSringData*>(m_implementationData);
                hashStringData->SetData(object);
            }
        }

        const std::array<HashString, nrHashes> GetData() const
        {
            if (m_implementationData != nullptr)
            {
                auto* hashStringData = static_cast<DoubleHashSringData*>(m_implementationData);
                return hashStringData->GetData();
            }

            return std::array<HashString, nrHashes>();
        }
    };

    class CreateScreen : public FEMessage 
    {
    public:
        CreateScreen()
        {
            m_MessageId = "CreateScreen"_hash;
            m_implementationData = new HashStringData();
            m_implementationDataSize = sizeof(HashStringData);
        }

        ~CreateScreen() {}

        void SetObject(const HashString& screen)
        {
            if (m_implementationData != nullptr)
            {
                auto* hashStringData = static_cast<HashStringData*>(m_implementationData);
                hashStringData->SetData(screen);
            }
        }

        const HashString GetData() const
        {
            if (m_implementationData != nullptr)
            {
                auto* hashStringData = static_cast<HashStringData*>(m_implementationData);
                return hashStringData->GetData();
            }

            return HashString();
        }
    };
}

}

//class Message
//{
//public:
//    explicit Message(const std::string& target) : m_target(target) {}
//    ~Message() {}
//    const std::string& getTarget() const { return m_target; }
//private:
//    std::string m_target;
//};
//
//
//class ActivationMessage : public Message
//{
//public:
//    ActivationMessage(const std::string& target, bool activate) : Message(target), m_activate(activate) {}
//    bool shouldActivate() const { return m_activate; }
//private:
//    bool m_activate;
//};
