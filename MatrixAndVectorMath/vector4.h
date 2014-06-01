#ifndef VECTOR4_H
#define VECTOR4_H

#include "..\SpaceSim\AlignMacros.h"
#include <vector3.h>
#include <iostream>

class ALIGN16 Vector4
{
public:
	Vector4();
    Vector4(const Vector3& v, float w);
	Vector4(float x, float y, float z, float w);
	Vector4(const float xyzw[]);
	~Vector4() {}

	float dot (const Vector4& v);
	float length();
	void normalize();
	Vector4 multiply(float scalar) const;
	Vector4 add(const Vector4& v) const;
	bool equal(const Vector4& v) const;
	void lerp(const Vector4& v0, float lerpVal)
	{
		m_x = v0.x() + ((m_x - v0.x()) * lerpVal);
		m_y = v0.y() + ((m_y - v0.y()) * lerpVal);
		m_z = v0.z() + ((m_z - v0.z()) * lerpVal);
		m_w = v0.w() + ((m_w - v0.w()) * lerpVal);
	}

	void lerp(const Vector4& v0, const Vector4& v1, float lerpVal)
	{
		m_x = v0.x() + ((v1.x() - v0.x()) * lerpVal);
		m_y = v0.y() + ((v1.y() - v0.y()) * lerpVal);
		m_z = v0.z() + ((v1.z() - v0.z()) * lerpVal);
		m_w = v0.w() + ((v1.w() - v0.w()) * lerpVal);
	}

	void operator+= (const Vector4& v);
	void operator-= (const Vector4& v);
	void operator*= (float scalar);
	void operator/= (float scalar);

	float x() const {return m_x;}
	float y() const {return m_y;}
	float z() const {return m_z;}
	float w() const {return m_w;}

	void read(std::istream& in);
	void write(std::ostream& out) const;

	static Vector4 zero() {return Vector4(0.0f, 0.0f, 0.0f, 0.0f);}
	static Vector4 xAxis() {return Vector4(1.0f, 0.0f, 0.0f, 1.0f);}
	static Vector4 yAxis() {return Vector4(0.0f, 1.0f, 0.0f, 1.0f);}
	static Vector4 zAxis() {return Vector4(0.0f, 0.0f, 1.0f, 1.0f);}
	static Vector4 wAxis() {return Vector4(0.0f, 0.0f, 0.0f, 1.0f);}
protected:
private:
	float m_x, m_y, m_z, m_w;
	static const float EQUALPRECISION4;
};

inline Vector4 operator- (const Vector4& v)
{
	return Vector4(-v.x(), -v.y(), -v.z(), -v.w());
}

inline Vector4 operator+ (const Vector4& lhs, const Vector4& rhs)
{
	return lhs.add(rhs);
}

inline Vector4 operator- (const Vector4& lhs, const Vector4& rhs)
{
	return lhs.add(rhs);
}

inline Vector4 operator* (const Vector4& lhs, float rhs)
{
	return lhs.multiply(rhs);
}

inline Vector4 operator* (float lhs, const Vector4& rhs)
{
	return rhs.multiply(lhs);
}

inline Vector4 operator/ (const Vector4& lhs, float rhs)
{
	return lhs.multiply(1/rhs);
}

inline Vector4 operator/ (float lhs, const Vector4& rhs)
{
	return rhs.multiply(1/lhs);
}

///Determines the minimum of two vectors used in BBoxes
inline Vector4 minv (const Vector4& a, const Vector4& b)
{
	return Vector4(
		a.x() < b.x() ? a.x() : b.x(),
		a.y() < b.y() ? a.y() : b.y(),
		a.z() < b.z() ? a.z() : b.z(),
		a.w() < b.w() ? a.w() : b.w());
}

///Determines the maximum of two vectors used in BBoxes
inline Vector4 maxv (const Vector4& a, const Vector4& b)
{
	return Vector4(
		a.x() > b.x() ? a.x() : b.x(),
		a.y() > b.y() ? a.y() : b.y(),
		a.z() > b.z() ? a.z() : b.z(),
		a.w() < b.w() ? a.w() : b.w());
}

inline std::ostream& operator<< (std::ostream& out, const Vector4& v)
{
	v.write(out);
	return out;
}

inline std::istream& operator>> (std::istream& in, Vector4& v)
{
	v.read(in);
	return in;
}

inline bool operator== (const Vector4& lhs, const Vector4& rhs)
{
	return lhs.equal(rhs);
}
#endif