#ifndef VECTOR3_H
#define VECTOR3_H

#include "Memory/AlignMacros.h"
#include "Core/tinyxml2.h"
#include "Math/vector2.h"
#include "Logging/LoggingMacros.h"
#include <array>
#include <iostream>

class Vector3
{
public:
    Vector3();
    Vector3(float x, float y, float z);
    Vector3(const float xyz[]);
    explicit Vector3(const Vector2& vec) : m_x(vec.x()), m_y(vec.y()), m_z(0.0f) {}
    ~Vector3() {}

    void cross (Vector3 &out, const Vector3& v);
    Vector3 cross (const Vector3& v) const;
    float dot (const Vector3& v) const;
    float length() const;
    void normalize();
    Vector3 findOrthogonal() const;

    void operator+= (const Vector3& v);
    void operator-= (const Vector3& v);
    void operator*= (float scalar);
    void operator/= (float scalar);

    Vector3 multiply(float scalar) const;
    Vector3 add(const Vector3& v) const;
    bool equal(const Vector3& v) const;

    float x() const {return m_x; }
    float y() const {return m_y; }
    float z() const {return m_z; }
    void setX(float x) {m_x = x;}
    void setY(float y) {m_y = y;}
    void setZ(float z) {m_z = z;}

    void deserialise( const tinyxml2::XMLElement* node );
    void read(std::istream& in);
    void write(std::ostream& out) const;

    float getEqualPrecision() const { return EQUALPRECISION3;}
    
    static Vector3 zero() {return Vector3(0.0f, 0.0f, 0.0f);}
    static Vector3 xAxis() {return Vector3(1.0f, 0.0f, 0.0f);}
    static Vector3 yAxis() {return Vector3(0.0f, 1.0f, 0.0f);}
    static Vector3 zAxis() {return Vector3(0.0f, 0.0f, 1.0f);}

    std::array<float, 3> toArray() const { return {m_x, m_y, m_z}; }
protected:
private:
    float m_x, m_y, m_z;
    static const float EQUALPRECISION3;
public:
    HASH_ELEMENT_DEFINITION(Vector3)
        void OnDebugImgui(const std::string_view& name);
};

inline Vector3 Vector3::findOrthogonal() const
{
     if (0.0 != m_x)
     {
         return Vector3((-m_y - m_z) / m_x, 1.0, 1.0);
     }
     else if (0.0 != m_y)
     {
         return Vector3(1.0, (-m_x - m_z) / m_y, 1.0);
     }
     else if (0.0 != m_z)
     {
         return Vector3(1.0, 1.0, (-m_x - m_y) / m_z);
     }
     else
     {
         return Vector3(0.0, 0.0, 0.0);
     }
 }

inline Vector3 operator- (const Vector3& v)
{
    return Vector3(-v.x(), -v.y(), -v.z());
}

inline Vector3 operator+ (const Vector3& lhs, const Vector3& rhs)
{
    return lhs.add(rhs);
}

inline Vector3 operator- (const Vector3& lhs, const Vector3& rhs)
{
    return lhs.add(-rhs);
}

inline Vector3 operator* (const Vector3& lhs, float rhs)
{
    return lhs.multiply(rhs);
}

inline Vector3 operator* (float lhs, const Vector3& rhs)
{
    return rhs.multiply(lhs);
}

inline Vector3 operator/ (const Vector3& lhs, float rhs)
{
    return lhs.multiply(1/rhs);
}

inline Vector3 operator/ (float lhs, const Vector3& rhs)
{
    return rhs.multiply(1/lhs);
}

inline void cross (Vector3& out, const Vector3& v1 ,const Vector3& v2)
{
    out = Vector3(v1.y()*v2.z() - v1.z()*v2.y(), v1.z()*v2.x() - v1.x()*v2.z(), v1.x()*v2.y() - v1.y()*v2.x());
}

inline Vector3 faceNormal (const Vector3& tobenormalled, const Vector3& v1 ,const Vector3& v2)
{
    Vector3 result, sub1, sub2;
    sub1 = tobenormalled -  v1;
    sub2 = tobenormalled -  v2;
    result = sub1.cross(sub2);
    result.normalize();
    return result;
}

///Determines the minimum of two vectors used in BBoxes
inline Vector3 minv (const Vector3& a, const Vector3& b)
{
    return Vector3(
        a.x() < b.x() ? a.x() : b.x(),
        a.y() < b.y() ? a.y() : b.y(),
        a.z() < b.z() ? a.z() : b.z());
}

///Determines the maximum of two vectors used in BBoxes
inline Vector3 maxv (const Vector3& a, const Vector3& b)
{
    return Vector3(
        a.x() > b.x() ? a.x() : b.x(),
        a.y() > b.y() ? a.y() : b.y(),
        a.z() > b.z() ? a.z() : b.z());
}

inline std::ostream& operator<< (std::ostream& out, const Vector3& v)
{
    v.write(out);
    return out;
}

inline std::istream& operator>> (std::istream& in, Vector3& v)
{
    v.read(in);
    return in;
}

inline bool operator== (const Vector3& lhs, const Vector3& rhs)
{
    return lhs.equal(rhs);
}

inline bool operator!= (const Vector3& lhs, const Vector3& rhs)
{
    return !lhs.equal(rhs);
}

inline float distance(const Vector3& v1, const Vector3& v2)
{
    Vector3 d = v2 - v1;
    return d.length();
}

inline float distanceSquared(const Vector3& v1, const Vector3& v2)
{
    Vector3 d = v2 - v1;
    return d.dot(d);
}
#endif