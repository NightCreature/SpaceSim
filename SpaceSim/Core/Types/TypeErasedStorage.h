#pragma once

#include <memory>

namespace TypeErasureDetails
{
struct StorageInterface
{
    virtual ~StorageInterface() {}

    virtual const void* GetData() const = 0;
    virtual size_t GetSize() const { return 0; }
};

template<class T>
struct StorageSingle : public StorageInterface
{
    StorageSingle() = default;
    StorageSingle(const T& data) : m_data(data) {}
    ~StorageSingle() = default;

    const void* GetData() const override { return static_cast<const void*>(&m_data); }
    size_t GetSize() const override { return 1; }

    T m_data;
};

template<class T, size_t size>
struct StorageArray : public StorageInterface
{
    StorageArray() = default;
    StorageArray(const T& data) : m_data(data) {}
    ~StorageArray() = default;

    const void* GetData() const override { return static_cast<const void*>(&m_data[0]); }
    size_t GetSize() const override { return size; }

    T m_data[size];
};

template<class T>
struct StorageDynamicArray : public StorageInterface
{
    StorageDynamicArray() = default;
    StorageDynamicArray(const T* data, size_t size) : m_size(size)
    {
        //Needs to copy the data here
        if (data != nullptr && size > 0)
        {
            m_data = new T[size];
            memcpy(m_data, data, size);
        }
    }

    StorageDynamicArray(const StorageDynamicArray& other) = delete;
    StorageDynamicArray(StorageDynamicArray&& other) : m_data(std::move(other.m_data)), m_size(other.m_size) {}

    ~StorageDynamicArray() override
    {
        delete[] m_data;
        m_size = 0;
    }

    const void* GetData() const override { return static_cast<const void*>(m_data); }
    size_t GetSize() const override { return m_size; }

    T* m_data = nullptr;
    size_t m_size = 0;
};

}

class TypeErasedStorage
{
public:
    TypeErasedStorage() = default;

    template<class T>
    TypeErasedStorage(const T& data) : m_data(new TypeErasureDetails::StorageSingle<T>(data)) {}

    template<class T>
    TypeErasedStorage(const T* data, size_t size) : m_data(new TypeErasureDetails::StorageDynamicArray<T>(data, size)) {}

    template<class T, size_t size>
    TypeErasedStorage(const T* data) : m_data(new TypeErasureDetails::StorageArray<T, size>(data)) {}

    TypeErasedStorage(TypeErasedStorage&& other) : m_data(std::move(other.m_data)) {}

    template <class T>
    const T* GetData() const { return m_data ? static_cast<const T*>(m_data.get()->GetData()) : nullptr; }

    size_t GetSize() const { return m_data ? m_data.get()->GetSize() : 0; }
private:
    std::unique_ptr<TypeErasureDetails::StorageInterface> m_data;
};
