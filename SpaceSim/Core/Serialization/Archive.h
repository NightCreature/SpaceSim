#pragma once

#include "Core/Types/Types.h"
#include "Core/FileSystem/File.h"
#include "Core/Serialization/SerializationRegister.h"

#include <array>
#include <filesystem>
#include <type_traits>

namespace Details
{

//Stolen from https://coliru.stacked-crooked.com/a/809883794331481a
template <class T>
concept default_erasable = requires(T * p) {
    std::destroy_at(p);
};

template <class T, class C, class A>
concept allocator_erasable = requires(A m, T * p) {
    //requires std::same_as<typename C::allocator_type, typename std::allocator_traits<A>::rebind_alloc<T>>;
    std::allocator_traits<A>::destroy(m, p);
};

template <class C>
concept allocator_aware = requires (C c) {
    { c.get_allocator() } -> std::same_as<typename C::allocator_type>;
};

template <class C>
struct is_basic_string : std::false_type {};

template <class C, class T, class A>
struct is_basic_string<std::basic_string<C, T, A>> : std::true_type {};

template <class C>
constexpr bool is_basic_string_v = is_basic_string<C>::value;

template <class T, class C>
concept erasable = (is_basic_string_v< std::remove_cv_t<C>> && default_erasable<T>)
    || (allocator_aware<C> && allocator_erasable<T, C, typename C::allocator_type>)
    || (!allocator_aware<C> && default_erasable<T>);

template <class T>
concept container = requires(T a, const T b)
{
    requires std::ranges::range<T>;
    requires std::regular<T>;
    requires std::swappable<T>;
    requires erasable<typename T::value_type, T>;
    requires std::same_as<typename T::reference, typename T::value_type&>;
    requires std::same_as<typename T::const_reference, const typename T::value_type&>;
    requires std::forward_iterator<typename T::iterator>;
    requires std::forward_iterator<typename T::const_iterator>;
    requires std::signed_integral<typename T::difference_type>;
    requires std::same_as<typename T::difference_type, typename std::iterator_traits<typename T::iterator>::difference_type>;
    requires std::same_as<typename T::difference_type, typename std::iterator_traits<typename T::const_iterator>::difference_type>;
    { a.begin() } -> std::same_as<typename T::iterator>;
    { a.end() } -> std::same_as<typename T::iterator>;
    { b.begin() } -> std::same_as<typename T::const_iterator>;
    { b.end() } -> std::same_as<typename T::const_iterator>;
    { a.cbegin() } -> std::same_as<typename T::const_iterator>;
    { a.cend() } -> std::same_as<typename T::const_iterator>;
    { a.size() } -> std::same_as<typename T::size_type>;
    { a.max_size() } -> std::same_as<typename T::size_type>;
    { a.empty() } -> std::convertible_to<bool>;
};


template<class T, class Archive>
concept IsSerialisable = requires (T x) { x.Serialize(std::declval<Archive>); };

//Insidious but this doesnt check if the derived type has this function only if T does.
template<class T, class Archive>
constexpr bool HasSerialiazeFuncPtr = requires (T * x, Archive archive) { IsSerialisable<T, Archive>; };

template<class T, class Archive>
constexpr bool HasSerialiazeFunc = requires (T x, Archive archive) { x.Serialize(archive); };

}



class Archive
{
public:
    void Close()
    {
        m_file.Close();
    }
    bool Open(const std::filesystem::path& filePath)
    {
        m_file.createFile(filePath, VFS::FileMode::OpenAndCreate);
        return m_file.IsValid();
    }

    template<class T> 
    void Read(T& value)
    {
        if constexpr (std::is_pointer_v<T> && Details::HasSerialiazeFuncPtr<T, Archive>)
        {
            SerializationTag tag;
            ReadValue(tag);

            auto* type = GetSerializationTypeByTag(tag);
            value = static_cast<T>(type->GetCreateFP()());

            value->Serialize(*this);
        }
        else if constexpr (Details::HasSerialiazeFunc<T, Archive>)
        {
            SerializationTag tag;
            ReadValue(tag);

            auto* type = GetSerializationTypeByTag(tag);
            auto* temp = static_cast<T>(type->GetCreateFP()()); //This is not awesome but should work

            value = *temp; //copy data
            delete temp;

            value.Serialize(*this);
        }
        else
        {
            ReadValue(value);
        }
    }

    template <class T, size_t N>
    void Read(T(&value)[N])
    {
        size_t count = 0;
        m_file.Read(reinterpret_cast<const byte*>(&count), sizeof(size_t));
        assert(count == N);
        m_file.Write(reinterpret_cast<const byte*>(value), sizeof(T) * N);
    }

    template<Details::container Container>
    void Read(Container& container)
    {
        byte* data = nullptr;
        m_file.Read(data, sizeof(Container::size_type));
        typename Container::size_type bounds = *reinterpret_cast<Container::size_type*>(data);
        for (typename Container::size_type index = 0; index < bounds; ++index)
        {
            typename Container::value_type item{};
            Read(item);
            container.insert(end(container), item);
            MSG_TRACE_CHANNEL("Archive", "Reading index: %d and bounds: %d", index, bounds);
        }
    }

    //template<class T>
    //void Write(const T value)
    //{
    //    if constexpr (std::is_pointer_v<T> && Details::HasSerialiazeFuncPtr<T, Archive>)
    //    {
    //        value->Serialize(*this);
    //    }
    //    if constexpr (Details::HasSerialiazeFunc<T, Archive>)
    //    {
    //        value.Serialize(*this);
    //    }
    //    else
    //    {
    //        m_file.Write(reinterpret_cast<const byte*>(&value), sizeof(T));
    //    }
    //}

    template<class T>
    void Write(T& value)
    {
        if constexpr (std::is_pointer_v<T> && Details::HasSerialiazeFuncPtr<T, Archive>)
        {
            m_file.Write(reinterpret_cast<const byte*>(&(value->GetTag())), sizeof(value->GetTag()));
            value->Serialize(*this);
        }
        if constexpr (Details::HasSerialiazeFunc<T, Archive>)
        {
            m_file.Write(reinterpret_cast<const byte*>(&(value.GetTag())), sizeof(value.GetTag()));
            value.Serialize(*this);
        }
        else
        {
            m_file.Write(reinterpret_cast<const byte*>(&value), sizeof(T));
        }
    }

    template<class T>
    void WriteValue(T value)
    {
        if constexpr (std::is_pointer_v<T> && Details::HasSerialiazeFuncPtr<T, Archive>)
        {
            m_file.Write(reinterpret_cast<const byte*>(&(value->GetTag())), sizeof(value->GetTag()));
            value->Serialize(*this);
        }
        if constexpr (Details::HasSerialiazeFunc<T, Archive>)
        {
            m_file.Write(reinterpret_cast<const byte*>(&(value.GetTag())), sizeof(value.GetTag()));
            value.Serialize(*this);
        }
        else
        {
            m_file.Write(reinterpret_cast<const byte*>(&value), sizeof(T));
        }
    }


    //template<class T>
    //void Write(const T* value)
    //{
    //    if constexpr (Details::HasSerialiazeFuncPtr<T, Archive>)
    //    {
    //        value->Serialize(*this);
    //    }
    //    else
    //    {
    //        m_file.Write(reinterpret_cast<const byte*>(&*value), sizeof(T));
    //    }
    //}

    template <class T, size_t N>
    void Write(const T (&value)[N])
    {
        constexpr size_t count = N;
        m_file.Write(reinterpret_cast<const byte*>(&count), sizeof(size_t));
        m_file.Write(reinterpret_cast<const byte*>(value), sizeof(T) * N);
    }

    template<>
    void Write(const std::string& container)
    {
        typename std::string::size_type elementCount = container.size();
        m_file.Write(reinterpret_cast<const byte*>(&elementCount), sizeof(std::string::size_type));
        for (typename std::string::const_iterator it = container.cbegin(); it != container.cend(); ++it)
        {
            Write(*it);
        }
    }

    template<Details::container Container>
    void Write(Container& container)
    {
        typename Container::size_type elementCount = container.size();
        m_file.Write(reinterpret_cast<const byte*>(&elementCount), sizeof(Container::size_type));
        for (typename Container::const_iterator it = container.cbegin(); it != container.cend(); ++it)
        {
            if constexpr(std::is_pointer_v<typename Container::value_type>)
            {
                const auto* value = *it;
                Write(value); //The iterator has a value type here
            }
            else
            {
                Write(*it);
            }
        }
    }

private:
    template <class T>
    void ReadValue(T& value)
    {
        if constexpr (Details::HasSerialiazeFunc<T, Archive>)
        {
            value.Serialize(*this);
        }
        else
        {
            byte* data = nullptr;
            m_file.Read(data, sizeof(T));
            value = *(reinterpret_cast<T*>(data));
        }
    }


    template<class T>
    void ReadPtr(T* value)
    {
        if constexpr (Details::HasSerialiazeFuncPtr<T, Archive>)
        {
            value->Serialize(*this);
        }
        else
        {
            byte* data = nullptr;
            m_file.Read(data, sizeof(T));
            //Not sure this should allocate the pointer or not probably not
            memcpy(static_cast<void*>(value), static_cast<void*>(data), sizeof(T));
        }
    }

    template <class T, size_t N>
    void ReadArray(T(&value)[N])
    {
        byte* data = nullptr;
        m_file.Read(data, sizeof(size_t));
        size_t arraySize = static_cast<size_t>(*data);

        assert(arraySize == N);

        data = nullptr;
        m_file.Read(value, sizeof(T) * N);
    }

    VFS::File m_file;
};
