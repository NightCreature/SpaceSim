#pragma once

#include "assert.h"
#include <string>

namespace StringUtils
{

    template<class T, class Container >
    class Iterator : public std::iterator<std::random_access_iterator_tag, T>
    {
    public:
        Iterator()  : m_index(0), m_container(nullptr) {} //invalid container needs to be assigned before use
        explicit Iterator( const size_t index, Container* container ) : m_index(index), m_container(container) {}
        Iterator( const Iterator& other ) : m_index(other.m_index), m_container(other.m_container) {}
        Iterator& operator=( const Iterator& other ) { m_index = other.m_index; m_container = other.m_container; return *this;}
        reference operator*() const { assert(m_index <= m_container->size()); return m_container->at(m_index); }

        pointer operator->() const { assert(m_index <= m_container->size()); return &m_container[m_index]; }

        T operator[]( difference_type n ) const { assert(m_index + n < m_container->size()); return m_container[m_index + n]; }
        //prefix
        Iterator& operator++() { assert(m_index + 1 <= m_container->size()); ++m_index; return *this; }
        Iterator& operator--() { assert(m_index - 1 > 0); --m_index; return *this; }
        //postfix
        Iterator operator++( int ) { assert(m_index + 1 <= m_container->size()); Iterator temp = *this; ++m_index; return temp; }
        Iterator operator--( int ) { assert(m_index - 1 > 0); Iterator temp = *this; --m_index; return temp; }

        Iterator operator+( difference_type n ) const { return Iterator(*this) += n; }
        Iterator operator-( difference_type n ) const { return Iterator(*this) -= n; }
        Iterator& operator+=( difference_type n ) { assert(m_index + n <= m_container->size()); m_index += n; return *this; }
        Iterator& operator-=( difference_type n ) { assert(m_index - n > 0); m_index -= n; return *this; }

        bool equal(const Iterator& rhs) const { return m_container == rhs.m_container && m_index == rhs.m_index; }
        bool less(const Iterator& rhs) const { return m_container == rhs.m_container && m_index < rhs.m_index; }
        bool greater(const Iterator& rhs) const { return m_container == rhs.m_container && m_index > rhs.m_index; }
    private:
        size_t m_index;
        Container* m_container;
    };

    //container end and not initialised iterator should compare to true
    template<class T, class Container>
    Iterator<T, Container> operator+ (Iterator<T, Container> lhs, const Iterator<T, Container>& rhs) { return lhs += rhs; }
    template<class T, class Container>
    Iterator<T, Container> operator- (Iterator<T, Container> lhs, const Iterator<T, Container>& rhs) { return lhs -= rhs; }
    template<class T, class Container>
    bool operator== (const Iterator<T, Container>& lhs, const Iterator<T, Container>& rhs) { return lhs.equal(rhs); }
    template<class T, class Container>
    bool operator!= (const Iterator<T, Container>& lhs, const Iterator<T, Container>& rhs) { return !lhs.equal(rhs); }
    template<class T, class Container>
    bool operator< (const Iterator<T, Container>& lhs, const Iterator<T, Container>& rhs) { return lhs.less(rhs); }
    template<class T, class Container>
    bool operator<= (const Iterator<T, Container>& lhs, const Iterator<T, Container>& rhs) { return lhs.less(rhs) || lhs.equal(rhs); }
    template<class T, class Container>
    bool operator> (const Iterator<T, Container>& lhs, const Iterator<T, Container>& rhs) { return lhs.greater(rhs); }
    template<class T, class Container>
    bool operator>= (const Iterator<T, Container>& lhs, const Iterator<T, Container>& rhs) { return lhs.greater(rhs) || lhs.equal(rhs); }

    template<class T, class Container >
    class ConstIterator : public std::iterator<std::random_access_iterator_tag, T>
    {
    public:
        ConstIterator()  : m_index(0), m_container(nullptr) {} //invalid container needs to be assigned before use
        explicit ConstIterator( const size_t index, const Container* container ) : m_index(index), m_container(container) {}
        ConstIterator( const ConstIterator& other ) : m_index(other.m_index), m_container(other.m_container) {}
        ConstIterator& operator=( const ConstIterator& other ) { m_index = other.m_index; m_container = other.m_container; return *this;}
        const T operator*() const { assert(m_index <= m_container->size()); return m_container->at(m_index); }

        const pointer operator->() const { assert(m_index <= m_container->size()); return &m_container[m_index]; }

        T operator[]( difference_type n ) const { assert(m_index + n < m_container->size()); return m_container[m_index + n]; }
        //prefix
        ConstIterator& operator++() { assert(m_index + 1 <= m_container->size()); ++m_index; return *this; }
        ConstIterator& operator--() { assert(m_index - 1 > 0); --m_index; return *this; }
        //postfix
        ConstIterator operator++( int ) { assert(m_index + 1 <= m_container->size()); ConstIterator temp = *this; ++m_index; return temp; }
        ConstIterator operator--( int ) { assert(m_index - 1 > 0); ConstIterator temp = *this; --m_index; return temp; }

        ConstIterator operator+( difference_type n ) const { return ConstIterator(*this) += n; }
        ConstIterator operator-( difference_type n ) const { return ConstIterator(*this) -= n; }
        ConstIterator& operator+=( difference_type n ) { assert(m_index + n <= m_container->size()); m_index += n; return *this; }
        ConstIterator& operator-=( difference_type n ) { assert(m_index - n > 0); m_index -= n; return *this; }

        bool equal(const ConstIterator& rhs) const { return m_container == rhs.m_container && m_index == rhs.m_index; }
        bool less(const ConstIterator& rhs) const { return m_container == rhs.m_container && m_index < rhs.m_index; }
        bool greater(const ConstIterator& rhs) const { return m_container == rhs.m_container && m_index > rhs.m_index; }
    private:
        size_t m_index;
        const Container* m_container;
    };

    //container end and not initialised ConstIterator should compare to true
    template<class T, class Container>
    ConstIterator<T, Container> operator+ (ConstIterator<T, Container> lhs, const ConstIterator<T, Container>& rhs) { return lhs += rhs; }
    template<class T, class Container>
    ConstIterator<T, Container> operator- (ConstIterator<T, Container> lhs, const ConstIterator<T, Container>& rhs) { return lhs -= rhs; }
    template<class T, class Container>
    bool operator== (const ConstIterator<T, Container>& lhs, const ConstIterator<T, Container>& rhs) { return lhs.equal(rhs); }
    template<class T, class Container>
    bool operator!= (const ConstIterator<T, Container>& lhs, const ConstIterator<T, Container>& rhs) { return !lhs.equal(rhs); }
    template<class T, class Container>
    bool operator< (const ConstIterator<T, Container>& lhs, const ConstIterator<T, Container>& rhs) { return lhs.less(rhs); }
    template<class T, class Container>
    bool operator<= (const ConstIterator<T, Container>& lhs, const ConstIterator<T, Container>& rhs) { return lhs.less(rhs) || lhs.equal(rhs); }
    template<class T, class Container>
    bool operator> (const ConstIterator<T, Container>& lhs, const ConstIterator<T, Container>& rhs) { return lhs.greater(rhs); }
    template<class T, class Container>
    bool operator>= (const ConstIterator<T, Container>& lhs, const ConstIterator<T, Container>& rhs) { return lhs.greater(rhs) || lhs.equal(rhs); }

template<class T, size_t Size = 64, class Traits = std::char_traits<T> > 
class StaticString
{
public:    
    typedef StaticString<T, Size, Traits> StaticStringType;
    typedef size_t size_type;
    typedef void allocator_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef Iterator<T, StaticStringType> iterator;
    typedef ConstIterator<T, StaticStringType> const_iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef const std::reverse_iterator<const_iterator> const_reverse_iterator;

    static const size_t npos = static_cast<size_t>(-1);

    StaticString() { reset(); }
    StaticString(const T* str) { assign(str); }
    //StaticString(const std::string& str) { assign(str); }
    StaticString(const StaticStringType& str) { assign(str); }
    ~StaticString() {}

    StaticString& append (const StaticString& str) { concatenate(str.c_str(), str.length()); }
    StaticString& append (const StaticString& str, size_t subpos, size_t sublen) { concatenate(str.c_str() + subpos, sublen); }
    StaticString& append (const T* s)  { concatenate(s, Traits::length(s)); }
    StaticString& append (const T* s, size_t n) { concatenate(s, n); }
    StaticString& append (size_t n, T c) 
    {
        StaticString str;
        str.assign(n, c);
        concatenate(str.c_str(), str.length());
    }
    template <class InputIterator>
    StaticString& append (InputIterator first, InputIterator last);
    //StaticString& append (initializer_list<T> il);

    StaticString& assign (const StaticString& str) { set(str.c_str(), str.size()); return *this; }
    StaticString& assign (const StaticString& str, size_t subpos, size_t sublen);
    StaticString& assign (const T* s)  { set(s, Traits::length(s)); return *this; }
    StaticString& assign (const T* s, size_t n) { set(s, n); return *this; }
    StaticString& assign (size_t n, T c) 
    {
        reset();
        m_length = n >= Size ? Size - 1 : n;
        memset((void*)m_buffer, c, m_length * sizeof(T));
        m_buffer[m_length] = 0;
        return *this;
    }
    template <class InputIterator>
    StaticString& assign (InputIterator first, InputIterator last);
    //StaticString& assign (initializer_list<T> il);
    //StaticString& assign (StaticString&& str) ;

    iterator begin() { return iterator(0, this); }
    const_iterator begin() const { return const_iterator(0, this); }
    iterator end() { return  iterator(m_length, this); }
    const_iterator end() const { return const_iterator(m_length, this); }
    const_iterator cbegin() const { return const_iterator(0u, this); }
    const_iterator cend() const { return const_iterator(m_length, this); }
    int compare (const StaticString& str) const 
    {
        if (str.size() != m_length)
        {
            return false;
        }

        return Traits::compare(m_buffer, str.c_str(), m_length) == 0;
    }
    int compare (size_t pos, size_t len, const StaticString& str) const
    {
        if (pos > m_length || len > m_length)
        {
            return false;
        }

        return Traits::compare(m_buffer + pos, str.c_str(), len) == 0;
    }
    int compare (size_t pos, size_t len, const StaticString& str, size_t subpos, size_t sublen) const;
    int compare (const T* s) const;
    int compare (size_t pos, size_t len, const T* s) const;
    int compare (size_t pos, size_t len, const T* s, size_t n) const;

    size_t copy (T* s, size_t len, size_t pos = 0) const;
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;
    const T* data() const { return c_str(); }
    
    StaticString& erase (size_t pos = 0, size_t len = npos);
    iterator erase (const_iterator p);
    iterator erase (const_iterator first, const_iterator last);

    size_t find (const StaticString& str, size_t pos = 0) const;
    size_t find (const T* s, size_t pos = 0) const;
    size_t find (const T* s, size_t pos, size_type n) const;
    size_t find (T c, size_t pos = 0) const;

    //Token based so assume all chars in a string as seperate matches
    size_t find_first_not_of (const StaticString& str, size_t pos = 0) const ;
    size_t find_first_not_of (const T* s, size_t pos = 0) const;
    size_t find_first_not_of (const T* s, size_t pos, size_t n) const;
    size_t find_first_not_of (T c, size_t pos = 0) const ;

    size_t find_first_of (const StaticString& str, size_t pos = 0) const ;
    size_t find_first_of (const T* s, size_t pos = 0) const;
    size_t find_first_of (const T* s, size_t pos, size_t n) const;
    size_t find_first_of (T c, size_t pos = 0) const ;

    size_t find_last_not_of (const StaticString& str, size_t pos = npos) const ;
    size_t find_last_not_of (const T* s, size_t pos = npos) const;
    size_t find_last_not_of (const T* s, size_t pos, size_t n) const;
    size_t find_last_not_of (T c, size_t pos = npos) const ;

    size_t find_last_of (const StaticString& str, size_t pos = npos) const ;
    size_t find_last_of (const T* s, size_t pos = npos) const;
    size_t find_last_of (const T* s, size_t pos, size_t n) const;
    size_t find_last_of (T c, size_t pos = npos) const ;
    //end Token based so assume all chars in a string as seperate matches

    allocator_type get_allocator() const {}

    StaticString& insert (size_t pos, const StaticString& str);
    StaticString& insert (size_t pos, const StaticString& str, size_t subpos, size_t sublen);
    StaticString& insert (size_t pos, const T* s);
    StaticString& insert (size_t pos, const T* s, size_t n);
    StaticString& insert (size_t pos,   size_t n, T c);
    iterator insert (const_iterator p, size_t n, T c);
    iterator insert (const_iterator p, T c);
    template <class InputIterator>
    iterator insert (iterator p, InputIterator first, InputIterator last);
    //StaticString& insert (const_iterator p, initializer_list<T> il);

    StaticString& operator+= (const StaticString& str) { return concatenate(str, Traits::length(str)); }
    StaticString& operator+= (const T* s) { return concatenate(s, Traits::length(s)); }
    StaticString& operator+= (T c) { return concatenate(s, Traits::length(s)); }
    //StaticString& operator+= (initializer_list<T> il);

    StaticString& operator= (const StaticString& str) { assign(str); return *this; }
    StaticString& operator= (const T* s) { assign(s); return *this; }
    StaticString& operator= (T c) { assign(c); return *this; }
    //StaticString& operator= (initializer_list<T> il);
    //StaticString& operator= (StaticString&& str) ;

    void pop_back() { m_buffer[m_length - 1] = 0; --m_length; };

    void push_back (T t) { concatenate(&t, 1); }

    reverse_iterator rbegin() ;
    const_reverse_iterator rbegin() const ;

    reverse_iterator rend() ;
    const_reverse_iterator rend() const ;

    StaticString& replace (size_t pos,        size_t len,        const StaticString& str);
    StaticString& replace (const_iterator i1, const_iterator i2, const StaticString& str);
    StaticString& replace (size_t pos,        size_t len,        const StaticString& str,
    size_t subpos, size_t sublen);
    StaticString& replace (size_t pos,        size_t len,        const T* s);
    StaticString& replace (const_iterator i1, const_iterator i2, const T* s);
    StaticString& replace (size_t pos,        size_t len,        const T* s, size_t n);
    StaticString& replace (const_iterator i1, const_iterator i2, const T* s, size_t n);
    StaticString& replace (size_t pos,        size_t len,        size_t n, T c);
    StaticString& replace (const_iterator i1, const_iterator i2, size_t n, T c);
    template <class InputIterator>
    StaticString& replace (const_iterator i1, const_iterator i2, InputIterator first, InputIterator last);
    //StaticString& replace (const_iterator i1, const_iterator i2, initializer_list<T> il);

    void reserve (size_t n = 0) {}
    
    size_t rfind (const StaticString& str, size_t pos = npos) const ;
    size_t rfind (const T* s, size_t pos = npos) const;
    size_t rfind (const T* s, size_t pos, size_t n) const;
    size_t rfind (T c, size_t pos = npos) const ;

    StaticString substr (size_t pos = 0, size_t len = npos) const;

    void swap (StaticString& str);

    const StaticString& Set(const T* str) { set(str, Traits::length(str)); return *this; }
    //const StaticString& Set(const std::string& str) { set(str.c_str(), str.size()); return *this; }
    const StaticString& Set(const StaticStringType& str) { set(str.m_buffer, str.m_length); return *this; }

    const T* c_str() const { return m_buffer; }
    //const std::wstring getWString() const;

    template<size_t USize>
    int equals(const StaticString<T, USize>& rhs) const
    {
        if (rhs.size() != m_length)
        {
            return false;
        }

        return Traits::compare(m_buffer, rhs.c_str(), m_length);
    }

    //Capacity functions:
    size_t capacity() const { return Size; }
    size_t size() const { return m_length; }
    size_t length() const { return size(); }
    size_t max_size() const { return capacity(); }
    void clear() { Set(""); }
    bool empty() const { return m_length == 0;}
    //these do nothing but are part of std::string interface
    void resize (size_t n) {} 
    void resize (size_t n, T c) {}

    //Element access:
    T& operator[] (size_t pos)
    {
        return at(pos);
    }
    const T& operator[] (size_t pos) const
    {
        return at(pos);
    }
    T& at(size_t pos)
    {
        if (pos < m_length)
        {
            return m_buffer[pos];
        }

        assert(pos < m_length);
        return m_buffer[Size-1];
    }
    const T& at(size_t pos) const
    {
        if (pos < m_length)
        {
            return m_buffer[pos];
        }

        assert(pos < m_length);
        return m_buffer[Size-1];
    }
    T& back() { return m_buffer[m_length - 1]; }
    const T& back() const { return m_buffer[m_length - 1]; }
    T& front() { return m_buffer[0]; }
    const T& front() const { return m_buffer[0]; }

    bool whiteSpace() const;

    StaticStringType sprintf(const char* message, ...);

    enum class StringSplitOptions { None, RemoveEmptyEntries };
    template<size_t tokenArraySize, size_t delimitersArraySize>
    void split(StaticStringType (&output)[tokenArraySize], T (&delimiters)[delimitersArraySize], StringSplitOptions options = StringSplitOptions::None)
    {
        UNUSEDPARAM(options);
        UNUSEDPARAM(output);
        MSG_TRACE("%d, %d", tokenArraySize, delimitersArraySize);
        size_t outputTokenCount = 0;
        for (auto delimiter : delimiters)
        {
            auto buffer = m_buffer;
            while (buffer != m_buffer + m_length)
            {
                auto firstInstance = Traits::find(buffer, m_length, delimiter);
                if (firstInstance != nullptr)
                {
                    output[outputTokenCount] = subrange(buffer, firstInstance + 1);
                    ++outputTokenCount;
                    buffer += ((firstInstance + 1) - buffer);
                }
                else
                {
                    output[outputTokenCount] = StaticStringType(buffer);
                    ++outputTokenCount;
                    break;
                }
            }
        }
        //need subrange function
    }

    template<size_t tokenArraySize>
    void split(StaticStringType (&output)[tokenArraySize], StaticString delimiters, StringSplitOptions options = StringSplitOptions::None)
    {

    }

    bool toBool() const;
    int  toInt() const;
    unsigned int toUint() const;
    float toFloat() const;
    double toDouble() const;

    void reset()
    {
        m_buffer[0] = 0;
        m_length = 0;
    }
private:
    void set(const T* str, size_t length)
    {
        reset();
        m_length = length >= Size ? Size - 1 : length;
        memcpy(m_buffer, str, m_length);
        m_buffer[m_length] = 0;
    }

    const StaticStringType& concatenate(const T* str, size_t length)
    {
        if (length + m_length > Size)
        {
            length = Size - m_length;
        }

        memcpy(&m_buffer[m_length], str, length);
        m_length += length;
        m_length = m_length >= Size ? Size - 1 : m_length;
        m_buffer[m_length] = 0;

        return *this;
    }

    StaticStringType subrange( const T* buffer, const T* firstInstance )
    {
        StaticStringType str;
        str.set(buffer, firstInstance - buffer);
        return str;
    }



    size_t m_length;
    T m_buffer[Size == 0 ? 1 : Size];

};

template<class T, size_t TSize, size_t USize > 
bool operator ==(const StaticString<T, TSize>& lhs, const StaticString<T, USize>& rhs) { return lhs.equals(rhs) == 0; }

}
typedef StringUtils::StaticString<char, 16> String16;
//typedef StringUtils::StaticString<char, 32> String32;
//typedef StringUtils::StaticString<char, 64> String64;
//typedef StringUtils::StaticString<char, 128> String128;
//typedef StringUtils::StaticString<char, 256> String256;

//typedef StaticString<wchar_t, 16> WString16;
