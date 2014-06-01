#include "matrix33.h"
#include "matrixmath.h"
#include <iostream>
#include <iomanip>




Matrix33::Matrix33()
{
	for (int i=0;i<3;i++)
		for (int j =0;j<3;j++)
				m_m[i][j] = 0.0f;
}

Matrix33::Matrix33(float a11, float a12, float a13, float a21, float a22, float a23, float a31, float a32, float a33)
{
	m_m[0][0] = a11;
	m_m[0][1] = a12;
	m_m[0][2] = a13;
	m_m[1][0] = a21;
	m_m[1][1] = a22;
	m_m[1][2] = a23;
	m_m[2][0] = a31;
	m_m[2][1] = a32;
	m_m[2][2] = a33;
}

Matrix33::Matrix33(Vector3 v1, Vector3 v2, Vector3 v3)
{
	m_m[0][0] = v1.x();
	m_m[0][1] = v1.y();
	m_m[0][2] = v1.z();
	m_m[1][0] = v2.x();
	m_m[1][1] = v2.y();
	m_m[1][2] = v2.z();
	m_m[2][0] = v3.x();
	m_m[2][1] = v3.y();
	m_m[2][2] = v3.z();
}

Matrix33::Matrix33(const float a[])
{
	m_m[0][0] = a[0];
	m_m[0][1] = a[1];
	m_m[0][2] = a[2];
	m_m[1][0] = a[3];
	m_m[1][1] = a[4];
	m_m[1][2] = a[5];
	m_m[2][0] = a[6];
	m_m[2][1] = a[7];
	m_m[2][2] = a[8];
}

Matrix33::Matrix33(const Matrix33& m)
{
	for (int i=0;i<3;i++)
		for (int j =0;j<3;j++)
			m_m[i][j] = m.m_m[i][j];
}

Matrix33::~Matrix33()
{
}

void Matrix33::identity()
{
	for (int i=0;i<3;i++)
		for (int j =0;j<3;j++)
		{
			if (i==j)
				m_m[i][j] = 1.0f;
			else
				m_m[i][j] = 0.0f;
		}
}

void Matrix33::transpose()
{
	float temp;
	temp = m_m[0][1];
	m_m[0][1] = m_m[1][0];
	m_m[1][0] = temp;

	temp = m_m[0][2];
	m_m[0][2] = m_m[2][0];
	m_m[2][0] = temp;

	temp = m_m[1][2];
	m_m[1][2] = m_m[2][1];
	m_m[2][1] = temp;
}

Matrix33 Matrix33::transposed() const
{
	return Matrix33(m_m[0][0], m_m[1][0], m_m[2][0],
				    m_m[0][1], m_m[1][1], m_m[2][1], 
				    m_m[0][2], m_m[1][2], m_m[2][2]);
}

float Matrix33::determinant() const
{
	return m_m[0][0]*m_m[1][1]*m_m[2][2] + m_m[0][1]*m_m[1][2]*m_m[2][0] + m_m[1][0]*m_m[2][1]*m_m[0][2]
		 - m_m[0][2]*m_m[1][1]*m_m[2][0] - m_m[1][0]*m_m[0][2]*m_m[2][2] - m_m[0][0]*m_m[2][1]*m_m[1][2];
}

Matrix33 Matrix33::cofactor()
{
	Matrix33 cofactormatrix;
	float minormatrix[4];
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			minor(i, j, minormatrix);
			if (0 == (i+j)%2)
				cofactormatrix.m_m[i][j] = determinant2x2(minormatrix);
			else
				cofactormatrix.m_m[i][j] = -determinant2x2(minormatrix);
		}
	return cofactormatrix;
}

void Matrix33::minor(int row, int colom, float theminor[])
{
	int index = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (row != i && colom != j && index < 5)
			{
				theminor[index] = m_m[i][j];
				index++;
			}
			else
            {
				if ( index > 4)
				{
                    MSG_TRACE_CHANNEL("ERROR", "The class has more memory then a 3x3 matrix should have" )
                }
            }
		}
	}
}

float* Matrix33::operator [](unsigned int i)
{
	if (i > 2)
		return 0;
	return m_m[i];
}

Matrix33 Matrix33::multiply(const Matrix33& matrix)
{
	return Matrix33(
		m_m[0][0]*matrix.m_m[0][0]+m_m[0][1]*matrix.m_m[1][0]+m_m[0][2]*matrix.m_m[2][0],		m_m[0][0]*matrix.m_m[0][1]+m_m[0][1]*matrix.m_m[1][1]+m_m[0][2]*matrix.m_m[2][1],		m_m[0][0]*matrix.m_m[0][2]+m_m[0][1]*matrix.m_m[1][2]+m_m[0][2]*matrix.m_m[2][2],
		m_m[1][0]*matrix.m_m[0][0]+m_m[1][1]*matrix.m_m[1][0]+m_m[1][2]*matrix.m_m[2][0],		m_m[1][0]*matrix.m_m[0][1]+m_m[1][1]*matrix.m_m[1][1]+m_m[1][2]*matrix.m_m[2][1],		m_m[1][0]*matrix.m_m[0][2]+m_m[1][1]*matrix.m_m[1][2]+m_m[1][2]*matrix.m_m[2][2],
		m_m[2][0]*matrix.m_m[0][0]+m_m[2][1]*matrix.m_m[1][0]+m_m[2][2]*matrix.m_m[2][0],		m_m[2][0]*matrix.m_m[0][1]+m_m[2][1]*matrix.m_m[1][1]+m_m[2][2]*matrix.m_m[2][1],		m_m[2][0]*matrix.m_m[0][2]+m_m[2][1]*matrix.m_m[1][2]+m_m[2][2]*matrix.m_m[2][2]);
}

Matrix33 Matrix33::multiply(float scalar)
{
	return Matrix33(
		m_m[0][0]*scalar, m_m[0][1]*scalar, m_m[0][2]*scalar, 
		m_m[1][0]*scalar, m_m[1][1]*scalar, m_m[1][2]*scalar, 
		m_m[2][0]*scalar, m_m[2][1]*scalar, m_m[2][2]*scalar);
}

float Matrix33::determinant2x2(float matrix[4])
{
	return matrix[0]*matrix[3] - matrix[1]*matrix[2];
}

void Matrix33::writeM() const
{
    MSG_TRACE_CHANNEL("MATRIX3x3", "DX Matrix:");
    MSG_TRACE_CHANNEL("MATRIX3x3", "r1: %10.3f, %10.3f, %10.3f", m_m[0][0], m_m[0][1], m_m[0][2]);
    MSG_TRACE_CHANNEL("MATRIX3x3", "r2: %10.3f, %10.3f, %10.3f", m_m[1][0], m_m[1][1], m_m[1][2]);
    MSG_TRACE_CHANNEL("MATRIX3x3", "r3: %10.3f, %10.3f, %10.3f", m_m[2][0], m_m[2][1], m_m[2][2]);
}

