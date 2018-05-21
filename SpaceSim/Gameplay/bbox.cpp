#include "bbox.h"
#include "assert.h"
#include <limits>
#include <algorithm>



Bbox::Bbox()
{
	m_min = Vector3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
	m_max = Vector3(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
}

Bbox::~Bbox()
{
}

void Bbox::empty()
{
	m_min = Vector3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
	m_max = Vector3(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());
}

void Bbox::enclose(const Vector3 &v)
{
	if(v.x() < m_min.x()) m_min.setX(v.x());
	if(v.x() > m_max.x()) m_max.setX(v.x());
	if(v.y() < m_min.y()) m_min.setY(v.y());
	if(v.y() > m_max.y()) m_max.setY(v.y());
	if(v.z() < m_min.z()) m_min.setZ(v.z());
	if(v.z() > m_max.z()) m_max.setZ(v.z());
}

void Bbox::enclose(const Bbox &bbox)
{
	if(bbox.m_min.x() < m_min.x()) m_min.setX(bbox.m_min.x());
	if(bbox.m_max.x() > m_max.x()) m_min.setX(bbox.m_max.x());
	if(bbox.m_min.y() < m_min.y()) m_min.setY(bbox.m_min.y());
	if(bbox.m_max.y() > m_max.y()) m_min.setY(bbox.m_max.y());
	if(bbox.m_min.z() < m_min.z()) m_min.setZ(bbox.m_min.z());
	if(bbox.m_max.z() > m_max.z()) m_min.setZ(bbox.m_max.z());
}

void Bbox::setToTransformedBox(const Bbox &bbox, const Matrix44& transform)
{
	if (bbox.isEmpty())
	{
		empty();
		return;
	}
	m_min = m_max = Vector3(transform[3][0], transform[3][1], transform[3][2]);

	if(transform[0][0] > 0.0f)
	{
		float x = m_min.x();
		x += transform[0][0] * bbox.getMin().x();
		m_min.setX(x);
		x = m_max.x();
		x += transform[0][0] * bbox.getMax().x();
		m_max.setX(x);
	}
	else
	{
		float x = m_min.x();
		x += transform[0][0] * bbox.getMax().x();
		m_min.setX(x);
		x = m_max.x();
		x += transform[0][0] * bbox.getMin().x();
		m_max.setX(x);
	}
	if(transform[0][1] > 0.0f)
	{
		float y = m_min.y();
		y += transform[0][1] * bbox.getMin().x();
		m_min.setY(y);
		y = m_max.y();
		y+= transform[0][1] * bbox.getMax().x();
		m_max.setY(y);
	}
	else
	{
		float y = m_min.y();
		y += transform[0][1] * bbox.getMax().x();
		m_min.setY(y);
		y = m_max.y();
		y += transform[0][1] * bbox.getMin().x();
		m_max.setY(y);
	}
	if(transform[0][2] > 0.0f)
	{
		float z = m_min.z();
		z += transform[0][2] * bbox.getMin().x();
		m_min.setZ(z);
		z = m_max.z();
		z+= transform[0][2] * bbox.getMax().x();
		m_max.setZ(z);
	}
	else
	{
		float z = m_min.z();
		z += transform[0][2] * bbox.getMax().x();
		m_min.setZ(z);
		z = m_max.z();
		z += transform[0][2] * bbox.getMin().x();
		m_max.setZ(z);
	}

	if(transform[1][0] > 0.0f)
	{
		float x = m_min.x();
		x += transform[1][0] * bbox.getMin().y();
		m_min.setX(x);
		x = m_max.x();
		x += transform[1][0] * bbox.getMax().y();
		m_max.setX(x);
	}
	else
	{
		float x = m_min.x();
		x += transform[1][0] * bbox.getMax().y();
		m_min.setX(x);
		x = m_max.x();
		x += transform[1][0] * bbox.getMin().y();
		m_max.setX(x);
	}
	if(transform[1][1] > 0.0f)
	{
		float y = m_min.y();
		y += transform[1][1] * bbox.getMin().y();
		m_min.setY(y);
		y = m_max.y();
		y+= transform[1][1] * bbox.getMax().y();
		m_max.setY(y);
	}
	else
	{
		float y = m_min.y();
		y += transform[1][1] * bbox.getMax().y();
		m_min.setY(y);
		y = m_max.y();
		y += transform[1][1] * bbox.getMin().y();
		m_max.setY(y);
	}
	if(transform[1][2] > 0.0f)
	{
		float z = m_min.z();
		z += transform[1][2] * bbox.getMin().y();
		m_min.setZ(z);
		z = m_max.z();
		z+= transform[1][2] * bbox.getMax().y();
		m_max.setZ(z);
	}
	else
	{
		float z = m_min.z();
		z += transform[1][2] * bbox.getMax().y();
		m_min.setZ(z);
		z = m_max.z();
		z += transform[1][2] * bbox.getMin().y();
		m_max.setZ(z);
	}

	if(transform[2][0] > 0.0f)
	{
		float x = m_min.x();
		x += transform[2][0] * bbox.getMin().z();
		m_min.setX(x);
		x = m_max.x();
		x += transform[2][0] * bbox.getMax().z();
		m_max.setX(x);
	}
	else
	{
		float x = m_min.x();
		x += transform[2][0] * bbox.getMax().z();
		m_min.setX(x);
		x = m_max.x();
		x += transform[2][0] * bbox.getMin().z();
		m_max.setX(x);
	}
	if(transform[2][1] > 0.0f)
	{
		float y = m_min.y();
		y += transform[2][1] * bbox.getMin().z();
		m_min.setY(y);
		y = m_max.y();
		y+= transform[2][1] * bbox.getMax().z();
		m_max.setY(y);
	}
	else
	{
		float y = m_min.y();
		y += transform[2][1] * bbox.getMax().z();
		m_min.setY(y);
		y = m_max.y();
		y += transform[2][1] * bbox.getMin().z();
		m_max.setY(y);
	}
	if(transform[2][2] > 0.0f)
	{
		float z = m_min.z();
		z += transform[2][2] * bbox.getMin().z();
		m_min.setZ(z);
		z = m_max.z();
		z+= transform[2][2] * bbox.getMax().z();
		m_max.setZ(z);
	}
	else
	{
		float z = m_min.z();
		z += transform[2][2] * bbox.getMax().z();
		m_min.setZ(z);
		z = m_max.z();
		z += transform[2][2] * bbox.getMin().z();
		m_max.setZ(z);
	}
}

const Vector3 Bbox::closestPointTo(const Vector3 &v) const
{
	Vector3 closestpoint;

	if (v.x() < m_min.x())
		closestpoint.setX(m_min.x());
	else if (v.x() > m_max.x())
		closestpoint.setX(m_max.x());
	else
		closestpoint.setX(v.x());

	if (v.y() < m_min.y())
		closestpoint.setY(m_min.y());
	else if (v.y() > m_max.y())
		closestpoint.setY(m_max.y());
	else		closestpoint.setY(v.y());

	if (v.z() < m_min.z())
		closestpoint.setZ(m_min.z());
	else if (v.z() > m_max.z())
		closestpoint.setZ(m_max.z());
	else
		closestpoint.setZ(v.z());

	return closestpoint;
}

bool Bbox::intersectSphere(const Vector3 &center, float radius) const
{
	Vector3 closestpoint = closestPointTo(center);
	//distance squared between v and closestpoint < radius squared
	// dot product of closespoint - v
	return distanceSquared(center, closestpoint) < radius * radius;
}

Bbox::sideplane Bbox::classifyPlane(const Vector3 &normal, float d) const
{//if normal is known d can be computed for any point in the plane
	float mind, maxd;

	if (normal.x() > 0.0f)
	{
		mind = normal.x() * m_min.x();
		maxd = normal.x() * m_max.x();
	}
	else
	{
		mind = normal.x() * m_max.x();
		maxd = normal.x() * m_min.x();
	}

	if (normal.y() > 0.0f)
	{
		mind = normal.y() * m_min.y();
		maxd = normal.y() * m_max.y();
	}
	else
	{
		mind = normal.y() * m_max.y();
		maxd = normal.y() * m_min.y();
	}

	if (normal.z() > 0.0f)
	{
		mind = normal.z() * m_min.z();
		maxd = normal.z() * m_max.z();
	}
	else
	{
		mind = normal.z() * m_max.z();
		maxd = normal.z() * m_min.z();
	}

	if (mind >= d)
		return sideplane::FRONT;
	if (maxd <= d)
		return sideplane::BACK;
	return sideplane::intERSECT;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void Bbox::resetToBox(const Bbox& otherBox)
{
    m_min = otherBox.m_min;
    m_max = otherBox.m_max;
}

//Returns INF if it does not intersect
float Bbox::intersectPlane(const Vector3 &normal, float planed, const Vector3 &dir) const
{
	assert(fabs(normal.dot(normal) - 1.0f) < normal.getEqualPrecision());
	assert(fabs(dir.dot(dir) - 1.0f) < normal.getEqualPrecision());

	float inf = std::numeric_limits<float>::infinity();

	float dot = normal.dot(dir);
	if (dot >= 0.0f)
		return inf;
	float mind, maxd;
	if (normal.x() > 0.0f)
	{
		mind = normal.x() * m_min.x();
		maxd = normal.x() * m_max.x();
	}
	else
	{
		mind = normal.x() * m_max.x();
		maxd = normal.x() * m_min.x();
	}

	if (normal.y() > 0.0f)
	{
		mind += normal.y() * m_min.y();
		maxd += normal.y() * m_max.y();
	}
	else
	{
		mind += normal.y() * m_max.y();
		maxd += normal.y() * m_min.y();
	}

	if (normal.z() > 0.0f)
	{
		mind += normal.z() * m_min.z();
		maxd += normal.z() * m_max.z();
	}
	else
	{
		mind += normal.z() * m_max.z();
		maxd += normal.z() * m_min.z();
	}
	if (maxd <= planed)
		return inf;
	float t = (planed - mind) / dot;
	if (t < 0.0f)
		return 0.0f;
	return t;
}

float Bbox::rayintersect(const Vector3 &rayorigin, const Vector3 &raydelta, Vector3 &outnormal) const
{
	float inf = std::numeric_limits<float>::infinity();

	bool inside = true;

	float xn = 0.0f, xt = 0.0f;
	if (rayorigin.x() < m_min.x())
	{
		xt = m_min.x() - rayorigin.x();
		if (xt > raydelta.x())
			return inf;
		xt /= raydelta.x();
		inside = false;
		xn = -1.0f;
	}
	else if(rayorigin.x() > m_max.x())
	{
		xt = m_max.x() - rayorigin.x();
		if (xt < raydelta.x())
			return inf;
		xt /= raydelta.x();
		inside = false;
		xn = 1.0f;
	}
	else
		xt=-1.0f;

	float yn = 0.0f, yt = 0.0f;
	if (rayorigin.y() < m_min.y())
	{
		yt = m_min.y() - rayorigin.y();
		if (yt > raydelta.y())
			return inf;
		yt /= raydelta.y();
		inside = false;
		yn = -1.0f;
	}
	else if(rayorigin.y() > m_max.y())
	{
		yt = m_max.y() - rayorigin.y();
		if (yt < raydelta.y())
			return inf;
		yt /= raydelta.y();
		inside = false;
		yn = 1.0f;
	}
	else
		yt=-1.0f;

	float zn = 0.0f, zt = 0.0f;
	if (rayorigin.z() < m_min.z())
	{
		zt = m_min.z() - rayorigin.z();
		if (zt > raydelta.z())
			return inf;
		zt /= raydelta.z();
		inside = false;
		zn = -1.0f;
	}
	else if(rayorigin.z() > m_max.z())
	{
		zt = m_max.z() - rayorigin.z();
		if (zt < raydelta.z())
			return inf;
		zt /= raydelta.z();
		inside = false;
		zn = 1.0f;
	}
	else
		zt=-1.0f;

	if(inside)
	{
		if (outnormal != Vector3::zero())
		{
			outnormal = -raydelta;
			outnormal.normalize();
		}
		return 0.0f;
	}

	int which = 0;
	float t = xt;
	if (yt > t)
	{
		which = 1;
		t =yt;
	}
	if (zt > t)
	{
		which = 2;
		t = zt;
	}

	switch (which)
	{
		case 0: //intersect with yz plane
			{
				float y = rayorigin.y() + raydelta.y() * t;
				if (y < m_min.y() || y > m_max.y())
					return inf;
				float z = rayorigin.z() + raydelta.z() * t;
				if (z < m_min.z() || z > m_max.z())
					return inf;

				if(outnormal != Vector3::zero())
				{
					outnormal.setX(xn);
					outnormal.setY(0.0f);
					outnormal.setZ(0.0f);
				}
			}break;
		case 1: //intersect with xz plane
			{
				float x = rayorigin.x() + raydelta.x() * t;
				if (x < m_min.x() || x > m_max.x())
					return inf;
				float z = rayorigin.z() + raydelta.z() * t;
				if (z < m_min.z() || z > m_max.z())
					return inf;

				if(outnormal != Vector3::zero())
				{
					outnormal.setX(0.0f);
					outnormal.setY(yn);
					outnormal.setZ(0.0f);
				}
			}break;
		case 2: //intersect with xy plane
			{
				float x = rayorigin.x() + raydelta.x() * t;
				if (x < m_min.x() || x > m_max.x())
					return inf;
				float y = rayorigin.y() + raydelta.y() * t;
				if (y < m_min.y() || y > m_max.y())
					return inf;

				if(outnormal != Vector3::zero())
				{
					outnormal.setX(0.0f);
					outnormal.setY(0.0f);
					outnormal.setZ(zn);
				}
			}break;
	}
	return t;
}

///-------------------------------------------------------------------------
// @brief 
//         Faster method should implementation prove to slow
//         Box box;		
//         for(unsigned char i = 0; i < 3; ++i)
//         {
//             box.min[i] = box.max[i] = xfrm.m[3][i];	
//             for(unsigned char j = 0; j < 3; ++j)
//             {
//                 const float low = xfrm.m[j][i] * this->min[j], 
//                     const float high = xfrm.m[j][i] * this->max[j];
//         
//                 box.min[i] += (low < high) ? low : high;
//                 box.max[i] += (low < high) ? high : low;
//             }
//         }
//         return(box);
///-------------------------------------------------------------------------
void Bbox::transformAccordingToMatrix( const Matrix44& transform )
{
    Vector3 corners[] = 
    {
        m_max,
        m_min,
        Vector3( m_max.x(), m_min.y(), m_min.z()),
        Vector3( m_max.x(), m_max.y(), m_min.z()),
        Vector3( m_min.x(), m_max.y(), m_min.z()),
        Vector3( m_min.x(), m_max.y(), m_max.z()),
        Vector3( m_min.x(), m_min.y(), m_max.z()),
        Vector3( m_max.x(), m_min.y(), m_max.z()),
    };

    m_min = Vector3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
    m_max = Vector3(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());

    for (size_t counter = 0; counter < 8; ++counter)
    {
        Vector4 corner =  corners[counter] * transform;
        enclose(Vector3(corner.x(), corner.y(), corner.z()));
    }
}


bool intersectBboxes(const Bbox& b1, const Bbox& b2, Bbox* intersectbox)
{
	if (b1.getMin().x() > b2.getMax().x())
		return false;
	if (b1.getMax().x() > b2.getMin().x())
		return false;
	if (b1.getMin().y() > b2.getMax().y())
		return false;
	if (b1.getMax().y() > b2.getMin().y())
		return false;
	if (b1.getMin().z() > b2.getMax().z())
		return false;
	if (b1.getMax().z() > b2.getMin().z())
		return false;

	if (intersectbox != 0)
	{
        float x = std::max(b1.getMin().x(), b2.getMin().x());
        float y = std::max(b1.getMin().y(), b2.getMin().y());
        float z = std::max(b1.getMin().z(), b2.getMin().z());
		intersectbox->setMin(Vector3( x, y, z ));
        x = std::min(b1.getMax().x(), b2.getMax().x());
        y = std::min(b1.getMax().y(), b2.getMax().y());
        z = std::min(b1.getMax().z(), b2.getMax().z());
		intersectbox->setMax(Vector3( x, y, z ));
	}

	return true;
}

float intersectMovingBboxes(const Bbox& stationarybox, const Bbox& movingbox, const Vector3& d)
{
	float inf = std::numeric_limits<float>::infinity();

	float tenter = 0.0f, tleave = 1.0f;
	if (d.x() == 0.0f)
	{
		if ((stationarybox.getMin().x() >= movingbox.getMax().x()) || (stationarybox.getMax().x() >= movingbox.getMin().x()))
			return inf;
	}
	else
	{
		float oneoverd = 1.0f / d.x();
		float xenter = (stationarybox.getMin().x() - movingbox.getMax().x()) * oneoverd;
		float xleave = (stationarybox.getMax().x() - movingbox.getMin().x()) * oneoverd;
		if (xenter > xleave)
			std::swap(xenter, xleave);
		if (xenter > tenter)
			tenter = xenter;
		if (xleave < tleave)
			tleave = xleave;
		if (tenter > tleave)
			return inf;
	}

	if (d.y() == 0.0f)
	{
		if ((stationarybox.getMin().y() >= movingbox.getMax().y()) || (stationarybox.getMax().y() >= movingbox.getMin().y()))
			return inf;
	}
	else
	{
		float oneoverd = 1.0f / d.y();
		float yenter = (stationarybox.getMin().y() - movingbox.getMax().y()) * oneoverd;
		float yleave = (stationarybox.getMax().y() - movingbox.getMin().y()) * oneoverd;
		if (yenter > yleave)
			std::swap(yenter, yleave);
		if (yenter > tenter)
			tenter = yenter;
		if (yleave < tleave)
			tleave = yleave;
		if (tenter > tleave)
			return inf;
	}

	if (d.z() == 0.0f)
	{
		if ((stationarybox.getMin().z() >= movingbox.getMax().z()) || (stationarybox.getMax().z() >= movingbox.getMin().z()))
			return inf;
	}
	else
	{
		float oneoverd = 1.0f / d.z();
		float zenter = (stationarybox.getMin().z() - movingbox.getMax().z()) * oneoverd;
		float zleave = (stationarybox.getMax().z() - movingbox.getMin().z()) * oneoverd;
		if (zenter > zleave)
			std::swap(zenter, zleave);
		if (zenter > tenter)
			tenter = zenter;
		if (zleave < tleave)
			tleave = zleave;
		if (tenter > tleave)
			return inf;
	}

	return tenter;
}

//Vector3 Bbox::getCenter() const
