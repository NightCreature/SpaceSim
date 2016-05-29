#ifndef MATRIX44_H
#define MATRIX44_H

#include "Memory/AlignMacros.h"
#include "Math/vector4.h"
#include "Math/vector3.h"
#include <cmath>

//Operators assume a row major implementation of this matrix so v * T ipv T * v for normal transformations
class ALIGN16 Matrix44
{
public:
	Matrix44();
	Matrix44(float a11, float a12, float a13, float a14, float a21, float a22, float a23, float a24, float a31, float a32, float a33, float a34, float a41, float a42, float a43, float a44);
	Matrix44(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4);
	~Matrix44();

	void transpose();
	Matrix44 transposed() const;
	bool invert();
	Matrix44 inverted() const;
	float determinant() const;
	Matrix44 cofactor();

	Vector4 getTransform();

	void identity();

	void operator*= (const Matrix44& matrix);
	void operator*= (float scalar);
	const float* operator[] (unsigned int i) const;

	const Vector3 getTranslate() const {return Vector3(m_m[0][3], m_m[1][3], m_m[2][3]);}
    void setTranslate(const Vector3& translate) { m_m[0][3] = translate.x(); m_m[1][3] = translate.y(); m_m[2][3] = translate.z(); }

	void writeM() const;
protected:
private:
	Matrix44 multiply (const Matrix44& m);
	Matrix44 multiply (float scalar);
	void minor(int row, int colom, float theminor[]);

    float m_m[4][4];
};

inline Matrix44 rotationFromDirection(Vector3 direction)
{
    direction.normalize();
    Vector3 up, right;
    up = Vector3::yAxis();
    cross(right, up, direction);
    right.normalize();
    cross(up, direction, right);
    up.normalize();
    return Matrix44(right.x(),      right.y(),     right.z(),      0.0f,
                     up.x(),         up.y(),         up.z(),         0.0f,
                     direction.x(), direction.y(), direction.z(), 0.0f,
                     0.0f,	         0.0f,	         0.0f,		      1.0f);
}

inline void rotateX(Matrix44& m, float angle)
{
	m = Matrix44(1.0f, 0.0f, 0.0f, 0.0f,
				 0.0f, cos(angle), sin(angle), 0.0f,
				 0.0f, -sin(angle), cos(angle), 0.0f,
				 0.0f, 0.0f, 0.0f, 1.0f);
}

inline void rotateY(Matrix44& m, float angle)
{
	m = Matrix44(cos(angle), 0.0f, -sin(angle), 0.0f,
				 0.0f, 1.0f, 0.0f, 0.0f,
				 sin(angle), 0.0f, cos(angle), 0.0f,
				 0.0f, 0.0f, 0.0f, 1.0f);
}

inline void rotateZ(Matrix44& m, float angle)
{
	m = Matrix44(cos(angle), sin(angle), 0.0f, 0.0f,
				 -sin(angle), cos(angle), 0.0f, 0.0f,
				 0.0f, 0.0f, 1.0f, 0.0f,
				 0.0f, 0.0f, 0.0f, 1.0f);
}

inline void rotate(Matrix44& m, const Vector3& v, float angle)
{
	//glRotatef(angle, x, y, z)
	//|0. x2(1 - c) + c		4. xy(1 - c) - zs  8. xz(1 - c) + ys 	12. 0 	|
	//|1. yx(1 - c) + zs    5. y2(1 - c) + c   9. yz(1 - c) - xs 	13. 0 	|
	//|2. xz(1 - c) - ys    6. yz(1 - c) + xs 10. z2(1 - c) + c 	14. 0 	|
	//|3. 		0 		    7.		0 		  11.		0			15. 1 	|
	//DX    = OpenGL = Value
	float rangle = angle * (3.14159265f / 180);
	m = Matrix44(v.x() * v.x() * (1 - cos(rangle)) + cos(rangle), v.x() * v.y() * (1 - cos(rangle)) - v.z() * sin(rangle), v.x() * v.z() * (1 - cos(rangle)) + v.y() * sin(rangle), 0.0f,
				 v.x() * v.y() * (1 - cos(rangle)) + v.z() * sin(rangle), v.y() * v.y() * (1 - cos(rangle)) + cos(rangle), v.y() * v.z() * (1 - cos(rangle)) - v.x() * sin(rangle), 0.0f,
				 v.x() * v.z() * (1 - cos(rangle)) - v.y() * sin(rangle), v.y() * v.z() * (1 - cos(rangle)) + v.x() * sin(rangle), v.z() * v.z() * (1 - cos(rangle)) + cos(rangle), 0.0f,
				 0.0f, 0.0f, 0.0f, 1.0f);
}

inline void scale(Matrix44& m, float factorx, float factory, float factorz)
{
    m = Matrix44(factorx, 0.0f, 0.0f, 0.0f,
        0.0f, factory, 0.0f, 0.0f,
        0.0f, 0.0f, factorz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

inline Matrix44 scale(float factorx, float factory, float factorz)
{
    return Matrix44(factorx, 0.0f, 0.0f, 0.0f,
        0.0f, factory, 0.0f, 0.0f,
        0.0f, 0.0f, factorz, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

inline void scale(Matrix44& m, const Vector3& v, float factor)
{
	m = Matrix44(1 + (factor - 1) * (v.x() * v.x()), (factor - 1) * (v.x() * v.y()), (factor - 1) * (v.x() * v.z()), 0.0f,
				 (factor - 1) * (v.x() * v.y()), 1 + (factor - 1) * (v.y() * v.y()), (factor - 1) * (v.y() * v.z()), 0.0f,
				 (factor - 1) * (v.x() * v.z()), (factor - 1) * (v.y() * v.z()), 1 + (factor - 1) * (v.z() * v.z()), 0.0f,
				 0.0f, 0.0f, 0.0f, 1.0f);
}

inline void translate(Matrix44& m, float factorx, float factory, float factorz)
{
	m = Matrix44(1.0f, 0.0f, 0.0f, 0.0f,
				 0.0f, 1.0f, 0.0f, 0.0f,
				 0.0f, 0.0f, 1.0f, 0.0f,
				 factorx, factory, factorz, 1.0f);
}

inline void translate(Matrix44& m, const Vector3& v)
{
	m = Matrix44(1.0f, 0.0f, 0.0f, 0.0f,
				 0.0f, 1.0f, 0.0f, 0.0f,
				 0.0f, 0.0f, 1.0f, 0.0f,
				 v.x(),v.y(),v.z(),1.0f);
}

inline Matrix44 translate(float factorx, float factory, float factorz)
{
    return Matrix44(1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     factorx, factory, factorz, 1.0f);
}   

inline Matrix44 translate(const Vector3& v)
{
    return Matrix44(1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 1.0f, 0.0f,
                     v.x(),v.y(),v.z(),1.0f);
}   

inline Vector4 multiply(const Vector4& lhs, const Matrix44& rhs)
{
	return Vector4(
		lhs.x()*rhs[0][0]+lhs.y()*rhs[1][0]+lhs.z()*rhs[2][0]+lhs.w()*rhs[3][0],
		lhs.x()*rhs[0][1]+lhs.y()*rhs[1][1]+lhs.z()*rhs[2][1]+lhs.w()*rhs[3][1],
		lhs.x()*rhs[0][2]+lhs.y()*rhs[1][2]+lhs.z()*rhs[2][2]+lhs.w()*rhs[3][2],
		lhs.x()*rhs[0][3]+lhs.y()*rhs[1][3]+lhs.z()*rhs[2][3]+lhs.w()*rhs[3][3]);
}

inline Vector4 operator* (const Vector4& lhs, const Matrix44& rhs)
{
    return multiply(lhs, rhs);
}

inline Vector4 operator* (const Vector3& lhs, const Matrix44& rhs)
{
    return multiply(Vector4(lhs, 1.0f), rhs);
}

inline Matrix44 operator* (const Matrix44& lhs, const Matrix44& rhs)
{
	return Matrix44(
		lhs[0][0]*rhs[0][0]+lhs[0][1]*rhs[1][0]+lhs[0][2]*rhs[2][0]+lhs[0][3]*rhs[3][0],		lhs[0][0]*rhs[0][1]+lhs[0][1]*rhs[1][1]+lhs[0][2]*rhs[2][1]+lhs[0][3]*rhs[3][1],		lhs[0][0]*rhs[0][2]+lhs[0][1]*rhs[1][2]+lhs[0][2]*rhs[2][2]+lhs[0][3]*rhs[3][2],	lhs[0][0]*rhs[0][3]+lhs[0][1]*rhs[1][3]+lhs[0][2]*rhs[2][3]+lhs[0][3]*rhs[3][3],
		lhs[1][0]*rhs[0][0]+lhs[1][1]*rhs[1][0]+lhs[1][2]*rhs[2][0]+lhs[1][3]*rhs[3][0],		lhs[1][0]*rhs[0][1]+lhs[1][1]*rhs[1][1]+lhs[1][2]*rhs[2][1]+lhs[1][3]*rhs[3][1],		lhs[1][0]*rhs[0][2]+lhs[1][1]*rhs[1][2]+lhs[1][2]*rhs[2][2]+lhs[1][3]*rhs[3][2],	lhs[1][0]*rhs[0][3]+lhs[1][1]*rhs[1][3]+lhs[1][2]*rhs[2][3]+lhs[1][3]*rhs[3][3],
		lhs[2][0]*rhs[0][0]+lhs[2][1]*rhs[1][0]+lhs[2][2]*rhs[2][0]+lhs[2][3]*rhs[3][0],		lhs[2][0]*rhs[0][1]+lhs[2][1]*rhs[1][1]+lhs[2][2]*rhs[2][1]+lhs[2][3]*rhs[3][1],		lhs[2][0]*rhs[0][2]+lhs[2][1]*rhs[1][2]+lhs[2][2]*rhs[2][2]+lhs[2][3]*rhs[3][2],	lhs[2][0]*rhs[0][3]+lhs[2][1]*rhs[1][3]+lhs[2][2]*rhs[2][3]+lhs[2][3]*rhs[3][3],
		lhs[3][0]*rhs[0][0]+lhs[3][1]*rhs[1][0]+lhs[3][2]*rhs[2][0]+lhs[3][3]*rhs[3][0],		lhs[2][0]*rhs[0][1]+lhs[3][1]*rhs[1][1]+lhs[3][2]*rhs[2][1]+lhs[3][3]*rhs[3][1],		lhs[3][0]*rhs[0][2]+lhs[3][1]*rhs[1][2]+lhs[3][2]*rhs[2][2]+lhs[3][3]*rhs[3][2],	lhs[3][0]*rhs[0][3]+lhs[3][1]*rhs[1][3]+lhs[3][2]*rhs[2][3]+lhs[3][3]*rhs[3][3]);
}

inline Matrix44 operator* (float lhs, Matrix44& rhs)
{
	return Matrix44(
		rhs[0][0]*lhs, rhs[0][1]*lhs, rhs[0][2]*lhs, rhs[0][3]*lhs, 
		rhs[1][0]*lhs, rhs[1][1]*lhs, rhs[1][2]*lhs, rhs[1][3]*lhs,
		rhs[2][0]*lhs, rhs[2][1]*lhs, rhs[2][2]*lhs, rhs[2][3]*lhs,
		rhs[3][0]*lhs, rhs[3][1]*lhs, rhs[3][2]*lhs, rhs[3][3]*lhs);
}

inline Matrix44 operator* (Matrix44& lhs, float rhs)
{
	return Matrix44(
		lhs[0][0]*rhs, lhs[0][1]*rhs, lhs[0][2]*rhs, lhs[0][3]*rhs, 
		lhs[1][0]*rhs, lhs[1][1]*rhs, lhs[1][2]*rhs, lhs[1][3]*rhs,
		lhs[2][0]*rhs, lhs[2][1]*rhs, lhs[2][2]*rhs, lhs[2][3]*rhs,
		lhs[3][0]*rhs, lhs[3][1]*rhs, lhs[3][2]*rhs, lhs[3][3]*rhs);
}
#endif