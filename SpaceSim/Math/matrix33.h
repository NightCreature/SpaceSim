#ifndef MATRIX33_H
#define MATRIX33_H

#include "Memory/AlignMacros.h"
#include "Math/vector3.h"
#include <cmath>

class Matrix33
{
public:
	Matrix33();
	Matrix33(float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33);
	Matrix33(Vector3 v1, Vector3 v2, Vector3 v3);
	Matrix33(const float a[]);
	Matrix33(const Matrix33& m);
	~Matrix33();

	void transpose();
	Matrix33 transposed() const;
	float determinant() const;
	void identity();
	Matrix33 cofactor();
	void operator*= (const Matrix33& matrix);
	void operator*= (float scalar);

	float* operator[] (size_t i);
	void writeM() const;
protected:
private:
	float m_m[3][3];

	Matrix33 multiply (const Matrix33& matrix);
	Matrix33 multiply (float scalar);
	void minor(int row, int colom, float theminor[]);
	float determinant2x2(float matrix[4]);
};

//inline Matrix33 rotationFromDirection(Vector3 direction)
//{
//    direction.normalize();
//    Vector3 up, right;
//    up = Vector3::yAxis();
//    cross(right, up, direction);
//    right.normalize();
//    cross(up, direction, right);
//    up.normalize();
//    return Matrix44(right.x(),      right.y(),     right.z(),
//                     up.x(),         up.y(),         up.z(),
//                     direction.x(), direction.y(), direction.z());
//}

inline void rotateX(Matrix33& m, float angle)
{
	m[0][0] = 1.0f;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[1][0] = 0.0f;
	m[1][1] = cos(angle);
	m[1][2] = sin(angle);
	m[2][0] = 0.0f;
	m[2][1] = -sin(angle);
	m[2][2] = cos(angle);
}

inline void rotateY(Matrix33& m, float angle)
{
	m[0][0] = cos(angle);
	m[0][1] = 0.0f;
	m[0][2] = -sin(angle);
	m[1][0] = 0.0f;
	m[1][1] = 1.0f;
	m[1][2] = 0.0f;
	m[2][0] = sin(angle);
	m[2][1] = 0.0f;
	m[2][2] = cos(angle);
}

inline void rotateZ(Matrix33& m, float angle)
{
	m[0][0] = cos(angle);
	m[0][1] = sin(angle);
	m[0][2] = 0.0f;
	m[1][0] = -sin(angle);
	m[1][1] = cos(angle);
	m[1][2] = 0.0f;
	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = 1.0f;
}

inline void rotate(Matrix33& m, const Vector3& v, float angle)
{
	//glRotatef(angle, x, y, z)
	//|0. x2(1 - c) + c		4. xy(1 - c) - zs  8. xz(1 - c) + ys 	12. 0 	|
	//|1. yx(1 - c) + zs    5. y2(1 - c) + c   9. yz(1 - c) - xs 	13. 0 	|
	//|2. xz(1 - c) - ys    6. yz(1 - c) + xs 10. z2(1 - c) + c 	14. 0 	|
	//|3. 		0 		    7.		0 		  11.		0			15. 1 	|
	float rangle = angle * (3.14159265f / 180);
	m = Matrix33(v.x() * v.x() * (1 - cos(rangle)) + cos(rangle), v.x() * v.y() * (1 - cos(rangle)) - v.z() * sin(rangle), v.x() * v.z() * (1 - cos(rangle)) + v.y() * sin(rangle),
				 v.x() * v.y() * (1 - cos(rangle)) + v.z() * sin(rangle), v.y() * v.y() * (1 - cos(rangle)) + cos(rangle), v.y() * v.z() * (1 - cos(rangle)) - v.x() * sin(rangle),
				 v.x() * v.z() * (1 - cos(rangle)) - v.y() * sin(rangle), v.y() * v.z() * (1 - cos(rangle)) + v.x() * sin(rangle), v.z() * v.z() * (1 - cos(rangle)) + cos(rangle));
}

inline void scale(Matrix33& m, float factorx, float factory, float factorz)
{
    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 3; j++)
        {
            m[i][j] = 0.0f;
        }
    }
	m[0][0] = factorx;
	m[1][1] = factory;
	m[2][2] = factorz;
}

inline void scale(Matrix33& m, const Vector3& v, float factor)
{
	m[0][0] = 1 + (factor - 1) * (v.x() * v.x());
	m[0][1] = (factor - 1) * (v.x() * v.y());
	m[0][2] = (factor - 1) * (v.x() * v.z());
	m[1][0] = (factor - 1) * (v.x() * v.y());
	m[1][1] = 1 + (factor - 1) * (v.y() * v.y());
	m[1][2] = (factor - 1) * (v.y() * v.z());
	m[2][0] = (factor - 1) * (v.x() * v.z());
	m[2][1] = (factor - 1) * (v.y() * v.z());
	m[2][2] = 1 + (factor - 1) * (v.z() * v.z());
}

inline Matrix33 operator-(Matrix33 m)
{
	return Matrix33(-m[0][0], -m[0][1], -m[0][2], 
					-m[1][0], -m[1][1], -m[1][2], 
					-m[2][0], -m[2][1], -m[2][2]);
}

//line vector * 3x3 matrix
inline Vector3 multiply(const Vector3& v, Matrix33& m)
{//Check implementation
	return Vector3(
		v.x()*m[0][0]+v.y()*m[1][0]+v.z()*m[2][0],
		v.x()*m[0][1]+v.y()*m[1][1]+v.z()*m[2][1],
		v.x()*m[0][2]+v.y()*m[1][2]+v.z()*m[2][2]);
}

inline Vector3 operator* (const Vector3& v, Matrix33& m)
{
	return multiply(v, m);
}

inline Matrix33 operator* (Matrix33& lhs, Matrix33& rhs)
{
	return Matrix33(
		lhs[0][0]*rhs[0][0]+lhs[0][1]*rhs[1][0]+lhs[0][2]*rhs[2][0],		lhs[0][0]*rhs[0][1]+lhs[0][1]*rhs[1][1]+lhs[0][2]*rhs[2][1],		lhs[0][0]*rhs[0][2]+lhs[0][1]*rhs[1][2]+lhs[0][2]*rhs[2][2],
		lhs[1][0]*rhs[0][0]+lhs[1][1]*rhs[1][0]+lhs[1][2]*rhs[2][0],		lhs[1][0]*rhs[0][1]+lhs[1][1]*rhs[1][1]+lhs[1][2]*rhs[2][1],		lhs[1][0]*rhs[0][2]+lhs[1][1]*rhs[1][2]+lhs[1][2]*rhs[2][2],
		lhs[2][0]*rhs[0][0]+lhs[2][1]*rhs[1][0]+lhs[2][2]*rhs[2][0],		lhs[2][0]*rhs[0][1]+lhs[2][1]*rhs[1][1]+lhs[2][2]*rhs[2][1],		lhs[2][0]*rhs[0][2]+lhs[2][1]*rhs[1][2]+lhs[2][2]*rhs[2][2]);
}

inline Matrix33 operator* (float scalar, Matrix33& m1)
{
	return Matrix33(
		m1[0][0]*scalar, m1[0][1]*scalar, m1[0][2]*scalar, 
		m1[1][0]*scalar, m1[1][1]*scalar, m1[1][2]*scalar, 
		m1[2][0]*scalar, m1[2][1]*scalar, m1[2][2]*scalar);
}

inline Matrix33 operator* (Matrix33& m1, float scalar)
{
	return Matrix33(
		m1[0][0]*scalar, m1[0][1]*scalar, m1[0][2]*scalar, 
		m1[1][0]*scalar, m1[1][1]*scalar, m1[1][2]*scalar, 
		m1[2][0]*scalar, m1[2][1]*scalar, m1[2][2]*scalar);
}
#endif