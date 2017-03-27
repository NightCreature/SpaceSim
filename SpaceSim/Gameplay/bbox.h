#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Math/vector3.h"
#include "Math/matrix44.h"
#include <iostream>

class Bbox
{
public:
	enum sideplane
	{
		FRONT,
		BACK,
		intERSECT,
	};

	Bbox();
	Bbox(const Vector3& min, const Vector3& max) : m_min(min), m_max(max) {}
	~Bbox();

	void empty();
	void enclose(const Vector3& v);
	void enclose(const Bbox& bbox);
	void setToTransformedBox(const Bbox& bbox, const Matrix44& transform);
    void transformAccordingToMatrix(const Matrix44& transform);

	///Is the bbox empty
	bool isEmpty() const 
	{
		return (m_min.x() > m_max.x() ||
			    m_min.y() > m_max.y() || 
				m_min.z() > m_max.z());
	}
	///does the bbox contain vector v
	bool contains(const Vector3& v) const
	{
		return v.x() >= m_min.x() && v.x() <= m_max.x() && 
			   v.y() >= m_min.y() && v.y() <= m_max.y() &&
			   v.z() >= m_min.z() && v.z() <= m_max.y();
	}
	///Does the bbox contain Bounding Box bbox
	bool contains(const Bbox& bbox) const
	{
		return contains(bbox.m_min) && contains(bbox.m_max);
	}
	///Does it intersect with a sphere
	bool intersectSphere(const Vector3& center, float radius) const;
	///returns the time stamp where they collide
	float intersectPlane(const Vector3& normal, float planed, const Vector3& dir) const;
	///Calculate the point closest to this bbox
	const Vector3 closestPointTo(const Vector3& v) const;
	///returns the time stamp where they collide
	float rayintersect(const Vector3& rayorigin, const Vector3& raydelta, Vector3& outnormal) const;
	///Classufy on which side of the plane this bbox is
	sideplane classifyPlane(const Vector3& normal, float d) const;

	///Calculate the center point of the box
	const Vector3 center() const {return (m_min + m_max) * 0.5f;}
	///Defines the width(x), height(y) and length(z) of the bbox
	const Vector3 size() const {return m_max - m_min;}
	///Defines the radius vector of the bbox
	const float radius() const {return size().length() * 0.5f;}

	const Vector3 getMin() const {return m_min;}
	const Vector3 getMax() const {return m_max;}
	void setMin(const Vector3& min) {m_min = min;}
	void setMax(const Vector3& max) {m_max = max;}

	Vector3 getCenter() const {return center();}

    void resetToBox(const Bbox& otherBox);
protected:
private:
	Vector3 m_min;
	Vector3 m_max;
};

bool intersectBboxes(const Bbox& b1, const Bbox& b2, Bbox* intersectbox = 0);
///returns the time stamp where they collide
float intersectMovingBboxes(const Bbox& stationarybox, const Bbox& movingbox, const Vector3& d);
#endif