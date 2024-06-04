#pragma once

#include "Core/Serialization/Archive.h"
#include "Core/Types/TypeHelpers.h"
#include "Core/Types/TypeErasedStorage.h"

#include <string>
#include <string_view>
#include <type_traits>



class NewSetting
{
    public:

    NewSetting(const std::string_view& name, const auto& value)
        : m_name(name), m_value(value)
    {
    }

    NewSetting(NewSetting&& other) noexcept
        : m_name(other.m_name), m_value(std::move(other.m_value))
    {
    }

    NewSetting& operator=(NewSetting&& other) noexcept
    {
        m_name = other.m_name;
        m_value = std::move(other.m_value);
        return *this;
    }

    std::string_view name() const
    {
        return m_name;
    }

    template<typename T>
    const T& value() const
    {
        T retVal = T{};
        const T* data = m_value.GetData();
        if (data)
        {
            retVal = *data;
        }
        return retVal;
    }

    void SetValue(const auto& value)
    {
        //Need to add a set value in type erased Storage
       // m_value = value;
    }

    void Serialize(Archive& ar, std::false_type tag) const
    {
        UNUSEDPARAM(tag);

        ar.WriteContainer(m_name);
        ar.Write(m_value);
    }

    void Serialize(const Archive& ar, std::true_type tag)
    {
        UNUSEDPARAM(tag);

        std::string name;
        ar.ReadContainer(name);
        ar.Read(m_value);
    }

private:
    //Don't allow copying, TypeErasedStorage is not copyable
    NewSetting(const NewSetting&) = delete;
    NewSetting& operator=(const NewSetting&) = delete;

    std::string m_name;
    TypeErasedStorage m_value;
};
