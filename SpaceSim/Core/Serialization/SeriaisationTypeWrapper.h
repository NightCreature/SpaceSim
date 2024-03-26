#pragma once

#include <typeinfo>

template<class T>
class SerialisationTypeWrapper
{
public:
    SerialisationTypeWrapper() {}
    const char* GetId() const
    {
        return m_typeInfo.name();
    }

    size_t GetSize() const
    {
        return sizeof(T);
    }

    static auto Create()
    {
        if constexpr (std::is_pointer_v<T>)
        {
            return new std::remove_pointer_t<T>();
        }
        else
        {
            return T();
        }
    }
private:
    const std::type_info& m_typeInfo = typeid(T());
};
