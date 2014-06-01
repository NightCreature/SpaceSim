#ifndef interpolateCurveRGBA_H
#define interpolateCurveRGBA_H

#include "matrixmath.h"

class interpolateCurveRGBA
{
public:
	interpolateCurveRGBA();
	interpolateCurveRGBA(const Vector4& keyFrameValue0, const Vector4& keyFrameValue1,
		const Vector4& keyFrameValue2, const Vector4& keyFrameValue3,
		const float keyFramePos1, const float keyFramePos2);
	void SetParameters(const Vector4& keyFrameValue0, const Vector4& keyFrameValue1,
		const Vector4& keyFrameValue2, const Vector4& keyFrameValue3,
		const float keyFramePos1, const float keyFramePos2);
	void SetParameters(const interpolateCurveRGBA& src);
	const Vector4& GetValue(float pos) const;

	enum
	{
		NumValues = 4,
	};
private:
	Vector4 keyFrameValues[NumValues];
	float keyFramePos1, keyFramePos2;
};

inline interpolateCurveRGBA::interpolateCurveRGBA() :
keyFramePos1(.2f),
keyFramePos2(.8f)
{
	keyFrameValues[0] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	keyFrameValues[1] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	keyFrameValues[2] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	keyFrameValues[3] = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

inline interpolateCurveRGBA::interpolateCurveRGBA(const Vector4& keyFrameValue0,
											 const Vector4& keyFrameValue1, const Vector4& keyFrameValue2,
											 const Vector4& keyFrameValue3, const float keyFramePos1,
											 const float keyFramePos2) :
keyFramePos1(keyFramePos1),
keyFramePos2(keyFramePos2)
{
	this->keyFrameValues[0] = keyFrameValue0;
	this->keyFrameValues[1] = keyFrameValue1;
	this->keyFrameValues[2] = keyFrameValue2;
	this->keyFrameValues[3] = keyFrameValue3;
}

inline void interpolateCurveRGBA::SetParameters(const Vector4& keyFrameValue0, const Vector4& keyFrameValue1,
									 const Vector4& keyFrameValue2, const Vector4& keyFrameValue3,
									 const float keyFramePos1, const float keyFramePos2)
{
	this->keyFrameValues[0] = keyFrameValue0;
	this->keyFrameValues[1] = keyFrameValue1;
	this->keyFrameValues[2] = keyFrameValue2;
	this->keyFrameValues[3] = keyFrameValue3;
	this->keyFramePos1 = keyFramePos1;
	this->keyFramePos2 = keyFramePos2;
}
inline void interpolateCurveRGBA::SetParameters(const interpolateCurveRGBA& src)
{
	this->keyFrameValues[0] = src.keyFrameValues[0];
	this->keyFrameValues[1] = src.keyFrameValues[1];
	this->keyFrameValues[2] = src.keyFrameValues[2];
	this->keyFrameValues[3] = src.keyFrameValues[3];
	this->keyFramePos1 = src.keyFramePos1;
	this->keyFramePos2 = src.keyFramePos2;
}

inline const Vector4& interpolateCurveRGBA::GetValue(float pos) const
{
//00347 _vector3::lerp(const _vector3& v0, float lerpVal)
//00348 {
//00349     x = v0.x + ((x - v0.x) * lerpVal);
//00350     y = v0.y + ((y - v0.y) * lerpVal);
//00351     z = v0.z + ((z - v0.z) * lerpVal);
//00352 }
//00353 
//00354 //------------------------------------------------------------------------------
//00357 inline
//00358 void
//00359 _vector3::lerp(const _vector3& v0, const _vector3& v1, float lerpVal)
//00360 {
//00361     x = v0.x + ((v1.x - v0.x) * lerpVal);
//00362     y = v0.y + ((v1.y - v0.y) * lerpVal);
//00363     z = v0.z + ((v1.z - v0.z) * lerpVal);
//00364 }
	//T lerp(const T &a, const T &b, const float pos) 
	//{
	//	return pos * a + (1 - pos) * b;
	//}
	static Vector4 linearValue;

	if (pos < keyFramePos1)
	{
		//linearValue.lerp(keyFrameValues[1], keyFrameValues[0], pos / keyFramePos1);
		linearValue = this->keyFrameValues[1];
		linearValue.lerp(this->keyFrameValues[0],
			(pos / this->keyFramePos1));
	}
	else if (pos < keyFramePos2)
	{
		//linearValue.lerp(keyFrameValues[2], keyFrameValues[1], (pos - keyFramePos1) / (keyFramePos2 - keyFramePos1));
		linearValue = this->keyFrameValues[2];
		linearValue.lerp(this->keyFrameValues[1],
			(pos-this->keyFramePos1) / (this->keyFramePos2-this->keyFramePos1));
	}
	else
	{
		//linearValue.lerp(keyFrameValues[3], keyFrameValues[2], (pos - keyFramePos2) / (1.0 - keyFramePos1));
		linearValue = this->keyFrameValues[3];
		linearValue.lerp(this->keyFrameValues[2],
			(pos-this->keyFramePos2) / (1.0f-this->keyFramePos2));
	}
	return linearValue;
}
#endif