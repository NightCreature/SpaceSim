#pragma once

#include <assert.h>
#include <malloc.h>

namespace Allocator
{

class AllocatorInterface
{
public:
    AllocatorInterface(size_t size)
    {
        m_start = malloc(size);
        m_size = size;

        m_usedMemory = 0;
        m_numAllocations = 0;
    }

    virtual ~AllocatorInterface()
    {
        assert(m_numAllocations == 0 && m_usedMemory == 0);

        free(m_start);
        m_start = nullptr;
        m_size = 0;
    }

    virtual void* allocate(size_t size, unsigned char alignment = 4) = 0;

    virtual void deallocate(void* p) = 0;

    void* startAddress() const
    {
        return m_start;
    }

    size_t getSize() const
    {
        return m_size;
    }

    size_t getUsedMemory() const
    {
        return m_usedMemory;
    }

    size_t getNumAllocations() const
    {
        return m_numAllocations;
    }

protected:
    void*         m_start;
    size_t        m_size;

    size_t        m_usedMemory;
    size_t        m_numAllocations;
};

template <class T> T* allocateNew(AllocatorInterface& allocator)
{
    return new (allocator.allocate(sizeof(T), __alignof(T))) T;
}

template <class T> T* allocateNew(AllocatorInterface& allocator, const T& t)
{
    return new (allocator.allocate(sizeof(T), __alignof(T))) T(t);
}

template<class T> void deallocateDelete(AllocatorInterface& allocator, T& object)
{
    object.~T();
    allocator.deallocate(&object);
}

template<class T> T* allocateArray(AllocatorInterface& allocator, size_t length)
{
    ASSERT(length != 0);

    unsigned char headerSize = sizeof(size_t) / sizeof(T);

    if (sizeof(size_t) % sizeof(T) > 0)
        headerSize += 1;

    //Allocate extra space to store array length in the bytes before the array
    T* p = ((T*)allocator.allocate(sizeof(T)*(length + headerSize), __alignof(T))) + headerSize;

    *(((size_t*)p) - 1) = length;

    for (size_t i = 0; i < length; i++)
        new (&p[i]) T;

    return p;
}

template<class T> void deallocateArray(AllocatorInterface& allocator, T* array)
{
    ASSERT(array != nullptr);

    size_t length = *(((size_t*)array) - 1);

    for (size_t i = 0; i < length; i++)
        array[i].~T();

    //Calculate how much extra memory was allocated to store the length before the array
    unsigned char headerSize = sizeof(size_t) / sizeof(T);

    if (sizeof(size_t) % sizeof(T) > 0)
        headerSize += 1;

    allocator.deallocate(array - headerSize);
}

//template<class T>
//class STLAllocator
//{
//public:
//    typedef size_t size_type;
//    typedef T* pointer;
//    typedef const T* const_pointer;
//    typedef T& reference;
//    typedef const T& const_reference;
//    typedef T value_type;
//
//    STLAllocator();
//    STLAllocator(const STLAllocator& other);
//
//    template<typename U>
//    STLAllocator(const STLAllocator<U>&);
//
//    STLAllocator<T>& operator=(const STLAllocator& other);
//
//    template<typename U>
//    STLAllocator& operator=(const STLAllocator<U>& other);
//
//    pointer address(reference x) const;
//    const_pointer address(const_reference x) const;
//
//    pointer allocate(size_type n, const void* hint = 0);
//    void deallocate(void* p, size_type n);
//
//    void construct(pointer p, const T& val);
//
//    //Has to be implemented for C++11 allocator
//    //template<typename U, typename... Args>
//    //void construct(U* p, Args&&... args);
//
//    void destroy(pointer p);
//
//    template<typename U>
//    void destroy(U* p);
//
//    size_type max_size() const;
//
//    template<typename U>
//    struct rebind
//    {
//        typedef STLAllocator<U> other;
//    };
//private:
//    size_t m_numberOfAllocations;
//    size_t m_usedMemory;
//};

};

