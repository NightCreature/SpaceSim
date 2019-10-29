#include "Math/vector3.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include <cmath>

const float Vector3::EQUALPRECISION3 = 0.00001f;



Vector3::Vector3()
{
    m_x = 0.0f;
    m_y = 0.0f;
    m_z = 0.0f;
}

Vector3::Vector3(float x, float y, float z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

Vector3::Vector3(const float xyz[])
{
    m_x = xyz[0];
    m_y = xyz[1];
    m_z = xyz[2];
}

void Vector3::cross (Vector3 &out, const Vector3& v)
{
    out.m_x = m_y*v.z() - m_z*v.y();
    out.m_y = m_z*v.x() - m_x*v.z();
    out.m_z = m_x*v.y() - m_y*v.x();
}

Vector3 Vector3::cross (const Vector3& v) const
{
    return Vector3(m_y*v.z() - m_z*v.y(), m_z*v.x() - m_x*v.z(), m_x*v.y() - m_y*v.x());
}

float Vector3::dot (const Vector3& v) const
{
    return (m_x*v.x() + m_y*v.y() + m_z*v.z());
}

float Vector3::length() const
{
    return sqrt(dot(*this));
}

void Vector3::normalize()
{
    *this = *this/length();
}

Vector3 Vector3::multiply(float scalar) const
{
    return Vector3(m_x * scalar, m_y * scalar, m_z * scalar);
}

Vector3 Vector3::add(const Vector3& v) const
{
    return Vector3(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
}

bool Vector3::equal(const Vector3& v) const
{
    if (fabs(m_x - v.x()) < EQUALPRECISION3 && fabs(m_y - v.y()) < EQUALPRECISION3 && fabs(m_z - v.z()) < EQUALPRECISION3)
        return true;
    return false;
}

void Vector3::operator+= (const Vector3& v)
{
    m_x += v.m_x;
    m_y += v.m_y;
    m_z += v.m_z;
}

void Vector3::operator-= (const Vector3& v)
{
    m_x -= v.m_x;
    m_y -= v.m_y;
    m_z -= v.m_z;
}


void Vector3::operator*= (float scalar)
{
    *this = multiply(scalar);
}

void Vector3::operator/= (float scalar)
{
    *this = multiply(1/scalar);
}

void Vector3::write(std::ostream &out) const
{
    out << "(" << m_x << ", " << m_y << ", " << m_z << ")";
}

void Vector3::read(std::istream &in)
{
    char ch;
    in >> ch >> m_x >> ch >> m_y >> ch >> m_z >> ch;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Vector3::deserialise( const tinyxml2::XMLElement* node )
{
    const tinyxml2::XMLAttribute* attribute = node->FindAttribute("x");
    if (attribute)
    {
        m_x = attribute->FloatValue();
    }
    attribute = node->FindAttribute("y");
    if (attribute)
    {
        m_y = attribute->FloatValue();
    }
    attribute = node->FindAttribute("z");
    if (attribute)
    {
        m_z = attribute->FloatValue();
    }
}
