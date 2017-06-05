#include "vector4.h"
#include <iostream>
#include <cmath>



const float Vector4::EQUALPRECISION4 = 0.00001f;

bool Vector4::equal(const Vector4& v) const
{
    if (fabs(m_x - v.m_x) < EQUALPRECISION4 && fabs(m_y - v.m_y) < EQUALPRECISION4 && fabs(m_z - v.m_z) < EQUALPRECISION4 && fabs(m_w - v.m_w) < EQUALPRECISION4)
        return true;
    return false;
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
