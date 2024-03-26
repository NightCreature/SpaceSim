#pragma once

#include "Core/Types/Types.h"
#include "Core/FileSystem/File.h"
#include "Core/Serialization/SerializationRegister.h"

#include <array>
#include <filesystem>
#include <typeinfo>
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

//Check if T has a serialise function that takes an Archive, const ref or normal ref
template<class T, class Archive>
constexpr bool HasSerialiseFunc = requires (T value, Archive & archive, std::bool_constant<true> writeOrRead) { value.Serialize(archive, writeOrRead); };
template<class T, class Archive>
constexpr bool HasSerialiseFuncConst = requires (T value, const Archive & archive, std::bool_constant<true> writeOrRead) { value.Serialize(archive, writeOrRead); };

template<class T, class Archive>
constexpr bool HasSerialiseFuncPtr = requires (T value, Archive & archive, std::bool_constant<true> writeOrRead) { value->Serialize(archive, writeOrRead); };
template<class T, class Archive>
constexpr bool HasSerialiseFuncPtrConst = requires (T value, const Archive & archive, std::bool_constant<true> writeOrRead) { value->Serialize(archive, writeOrRead); };

//Regardless of if T is a pointer or value, check if it has a serialise function
template<class T, class Archive>
concept IsSerialisable = HasSerialiseFunc<T, Archive> || HasSerialiseFuncConst<T, Archive> || HasSerialiseFuncPtr<T, Archive> || HasSerialiseFuncPtrConst<T, Archive>;
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

    template <class T>
    void Write(const T& value)
    {
        //First always write the type into the archive, so we can fetch this on read an know what to do
        size_t tag = GetOrCreateTag(value);
        m_file.Write(reinterpret_cast<byte*>(&tag), sizeof(size_t));

        if constexpr (Details::IsSerialisable<T, Archive>)
        {
            WriteSerialiseObject(value);
        }
        else
        {
            WriteObject(value);
        }
    }

    template<class T>
    void Read(T& value) const
    {
        byte* data = nullptr;
        m_file.Read(data, sizeof(size_t));
        size_t tag = static_cast<size_t>(*data);

        //From the tag we have to figure out what type we need to create
        if constexpr (Details::IsSerialisable<T, Archive>)
        {
            ReadSerialiseObject(value, tag);
        }
        else
        {
            ReadObject(value, tag);
        }
    }

    template<class Container>
    void WriteContainer(const Container& container)
    {
        size_t tag = GetOrCreateTag(container);
        m_file.Write(reinterpret_cast<byte*>(&tag), sizeof(size_t));

        size_t sizeContainer = container.size();
        m_file.Write(reinterpret_cast<byte*>(&sizeContainer), sizeof(size_t));

        for (auto iter = cbegin(container); iter != cend(container); ++iter)
        {
            Write(*iter);
        }
    }

    template<class Container>
    void ReadContainer(Container& container) const
    {
        byte* data = nullptr;
        m_file.Read(data, sizeof(size_t));
        size_t tag = static_cast<size_t>(*data);

        data = nullptr;
        m_file.Read(data, sizeof(size_t));
        size_t sizeContainer = static_cast<size_t>(*data);

        for (auto iter = cbegin(container); iter != cend(container); ++iter)
        {
            Container::value_type value;
            Read(value);
            container.insert(end(container), value);
        }
    }
private:
    template<class T>
    void WriteSerialiseObject(const T& value)
    {
        //Need to see if we have a pointer or not
        if constexpr (std::is_pointer_v<T>)
        {
            value->Serialize(*this, std::false_type());
        }
        else
        {
            value.Serialize(*this, std::false_type());
        }
    }

    template<class T>
    void WriteObject(const T& value)
    {
        if constexpr (std::is_fundamental_v<T>) //This cannot be a pointer or reference
        {
            m_file.Write(reinterpret_cast<const byte*>(&value), sizeof(T));
        }
        else if constexpr (std::is_pointer_v<T>)
        {
            m_file.Write(reinterpret_cast<const byte*>(&(*value)), sizeof(std::remove_pointer_t<T>));
        }
        else
        {

        }
    }

    template<class T>
    void ReadSerialiseObject(T& value, size_t tag) const
    {
        const SerializationType* serializationType = GetSerializationTypeByTag(tag);
        value = serializationType->template GetCreateFP<T>()(); //create an object here 

        //Need to see if we have a pointer or not
        if constexpr (std::is_pointer_v<T>)
        {
            value->Serialize(*this, std::true_type());
        }
        else
        {
            value.Serialize(*this, std::true_type());
        }
    }

    //Difficult option here
    template<class T>
    void ReadObject(T& value, size_t tag) const
    {
        const DefaultSerializationType<SerialisationTypeWrapper<T>>* serializationType = static_cast<const DefaultSerializationType<SerialisationTypeWrapper<T>>*>(GetSerializationTypeByTag(tag));
        value = serializationType->template GetCreateFP<T>()();
        //value = (); //create an object here

        if constexpr (std::is_fundamental_v<T>) //This cannot be a pointer or reference, also this doesnt need a type check
        {
            byte* data = nullptr;
            m_file.Read(data, sizeof(T));
            value = static_cast<T>(*data);
        }
        else if constexpr (std::is_pointer_v<T>)
        {
            byte* data = nullptr;
            m_file.Read(data, sizeof(std::remove_pointer_t<T>));
            value = reinterpret_cast<T*>(data); //This does not call a constructor
        }
        else
        {

        }
    }

    template<class T>
    size_t GetOrCreateTag(const T& value) const
    {
        auto typeName = typeid(T).name();
        
        const auto* serialisationTag = GetSerializationTagByNamePtr(typeName);
        if (serialisationTag != nullptr)
        {
            return *serialisationTag;
        }

        //create a unique tag here and store it
        //Need a new function here because the default type will not have certain things
        return RegisterNonSerialisationType<T>(typeName); //Caveat this only works if this is done in the same order as we read the data, might need to save this off problem for later
    }

    VFS::File m_file;
};

