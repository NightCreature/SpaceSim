#pragma once

#include <array>
#include <limits>

template <class T, size_t size>
class VectorBySize
{
public:
    using value_type = T;
    using vector_type = VectorBySize<T, size>;

    VectorBySize() { Set(0); }
    VectorBySize(T scalar) { Set(scalar); }
    //VectorBySize(const std::array<T, size>& data) { for (size_t counter = 0; counter < m_data.size() && counter < data.size(); ++counter) { m_data[counter] = data[counter] } }
    VectorBySize(const std::array<T, size>& data) : m_data(data) { }
    VectorBySize(const VectorBySize& v) { for (size_t counter = 0; counter < m_data.size(); ++counter) { m_data[counter] = v.Get(counter); } }
    ~VectorBySize() = default;


    value_type dot(const vector_type& v) const
    {
        value_type retVal = 0;
        for (size_t counter = 0; counter < size; ++counter)
        {
            retVal += Get(counter) * v.Get(counter);
        }

        return retVal;
    }

    value_type length()
    {
        return sqrt(dot(*this));
    }

    void normalize()
    {
        *this /= length();
    }

    vector_type multiply(T scalar) const
    {
        vector_type retVal = *this;
        for (size_t counter = 0; counter < size; ++counter)
        {
            retVal[counter] = m_data[counter] * scalar;
        }
        return retVal;
    }

    vector_type add(const vector_type& v) const
    {
        vector_type retVal = *this;
        for (size_t counter = 0; counter < size; ++counter)
        {
            retVal[counter] = m_data[counter] + v.Get(counter);
        }
        return retVal;
    }

    bool equal(const vector_type& v) const
    {
        bool retVal = true;
        for (size_t counter = 0; counter < size; ++counter)
        {
            T lhsVal = m_data[counter];
            T rhsVal = v.Get(counter);

            //For int types we want this to be long long (int64) for floats double
            if (std::is_integral_v<T>)
            {
                retVal &= lhsVal == rhsVal;
            }
            else if (std::is_floating_point_v<T>)
            {
                retVal &= std::abs(static_cast<double>((lhsVal - rhsVal))) <= std::numeric_limits<double>::epsilon();
            }
            else
            {
                static_assert(std::is_integral_v<T> || std::is_floating_point_v<T>, "incompatible type for vector by size");
            }
        }

        return retVal;
    }

    void operator+= (const vector_type& v)
    {
        *this = add(v);
    }

    void operator-= (const vector_type& v)
    {
        *this = add(-v);
    }

    void operator*= (T scalar)
    {
        *this = multiply(scalar);
    }

    void operator/= (T scalar)
    {
        *this = multiply(1 / scalar);
    }

    void Set(T scalar)
    {
        for (size_t counter = 0; counter < size; ++counter)
        {
            m_data[counter] = scalar;
        }
    }

    void Set(const vector_type& v)
    {
        for (size_t counter = 0; counter < size; ++counter)
        {
            m_data[counter] = v.Get(counter);
        }
    }

    const T& Get(const size_t index) const { return m_data[index]; }
    T& Get(const size_t index) { return m_data[index]; }
    const T& operator [](size_t index) const { return Get(index); }
    T& operator [](size_t index) { return Get(index); }
    constexpr size_t Size() const { return size; }

    std::array<T, size> GetData() const { return m_data; }
private:
    std::array<T, size> m_data;
};

template <class T, size_t size>
inline const VectorBySize<T, size> operator- (const VectorBySize<T, size>& lhs)
{ 
    VectorBySize<T, size> retVal;
    for (size_t counter = 0; counter < size; ++counter)
    {
        retVal[counter] = -(lhs.Get(counter));
    }

    return retVal;
}

template <class T, size_t size>
inline VectorBySize<T, size> operator+ (const VectorBySize<T, size>& lhs, const VectorBySize<T, size>& rhs)
{
    return lhs.add(rhs);
}

template <class T, size_t size>
inline VectorBySize<T, size> operator- (const VectorBySize<T, size>& lhs, const VectorBySize<T, size>& rhs)
{
    return lhs.add(-rhs);
}

template <class T, size_t size>
inline VectorBySize<T, size> operator* (const VectorBySize<T, size>& lhs, const T& rhs)
{
    return lhs.multiply(rhs);
}

template <class T, size_t size>
inline VectorBySize<T, size> operator* (const T& lhs, const VectorBySize<T, size>& rhs)
{
    return rhs.multiply(lhs);
}

template <class T, size_t size, T equalEpsilon>
inline VectorBySize<T, size> operator/ (const VectorBySize<T, size>& lhs, const T& rhs)
{
    return lhs.multiply(1 / rhs);
}

template <class T, size_t size, T equalEpsilon>
inline VectorBySize<T, size> operator/ (const T& lhs, const VectorBySize<T, size>& rhs)
{
    return rhs.multiply(1 / lhs);
}

template <class T, size_t size>
inline bool operator==(const T& lhs, const T& rhs)
{
    return lhs.equal(rhs);
}