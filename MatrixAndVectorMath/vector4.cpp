#include "vector4.h"
#include <iostream>
#include <cmath>



const float Vector4::EQUALPRECISION4 = 0.00001f;

Vector4::Vector4()
{
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
	m_w = 0.0f;
}

Vector4::Vector4(float x, float y, float z, float w)
{
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
}

Vector4::Vector4(const float xyzw[])
{
	m_x = xyzw[0];
	m_y = xyzw[1];
	m_z = xyzw[2];
	m_w = xyzw[3];
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
Vector4::Vector4( const Vector3& v, float w )
{
    m_x = v.x();
    m_y = v.y();
    m_z = v.z();
    m_w = w;
}

float Vector4::dot (const Vector4& v)
{
	return (m_x*v.m_x + m_y*v.m_y + m_z*v.m_z + m_w*v.m_w);
}

float Vector4::length()
{
	return sqrt(dot(*this));
}

void Vector4::normalize()
{
	*this = *this/length();
}

void Vector4::operator+= (const Vector4& v)
{
	m_x += v.x();
	m_y += v.y();
	m_z += v.z();
	m_w += v.w();
}


void Vector4::operator-= (const Vector4& v)
{
	m_x -= v.x();
	m_y -= v.y();
	m_z -= v.z();
	m_w -= v.w();
}

void Vector4::operator*= (float scalar)
{
	m_x *= scalar;
	m_y *= scalar;
	m_z *= scalar;
	m_w *= scalar;
}


void Vector4::operator/= (float scalar)
{
	m_x /= scalar;
	m_y /= scalar;
	m_z /= scalar;
	m_w /= scalar;
}

void Vector4::write(std::ostream &out) const
{
	out << "(" << m_x << ", " << m_y << ", " << m_z << ", " << m_w << ")";
}

void Vector4::read(std::istream &in)
{
	char ch;
	in >> ch >> m_x >> ch >> m_y >> ch >> m_z >> ch >> m_w >> ch;
}

Vector4 Vector4::multiply(float scalar) const
{
	return Vector4(m_x * scalar, m_y * scalar, m_z * scalar, m_w * scalar);
}

Vector4 Vector4::add(const Vector4& v) const
{
	return Vector4(m_x + v.x(), m_y + v.y(), m_z + v.z(), m_w + v.w());
}

bool Vector4::equal(const Vector4& v) const
{
	if (fabs(m_x - v.x()) < EQUALPRECISION4 && fabs(m_y - v.y()) < EQUALPRECISION4 && fabs(m_z - v.z()) < EQUALPRECISION4 && fabs(m_w - v.w()) < EQUALPRECISION4)
		return true;
	return false;
}
