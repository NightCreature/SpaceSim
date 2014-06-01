#pragma once

template<class T>
class Allocator
{
public:
    typedef size_t size_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    Allocator();
    Allocator(const Allocator& other);

    template<typename U>
    Allocator(const Allocator<U>&);

    Allocator<T>& operator=(const Allocator& other);

    template<typename U>
    Allocator& operator=(const Allocator<U>& other);

    pointer address(reference x) const;
    const_pointer address(const_reference x) const;

    pointer allocate(size_type n, const void* hint = 0);
    void deallocate(void* p, size_type n);

    void construct(pointer p, const T& val);

    //Has to be implemented for C++11 allocator
    //template<typename U, typename... Args>
    //void construct(U* p, Args&&... args);

    void destroy(pointer p);

    template<typename U>
    void destroy(U* p);

    size_type max_size() const;

    template<typename U>
    struct rebind
    {
        typedef Allocator<U> other;
    };
private:
    size_t m_numberOfAllocations;
    size_t m_usedMemory;
};

