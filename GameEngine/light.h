#ifndef LIGHT_H
#define LIGHT_H

#include "Vector3.h"
#include "Vector4.h"
#include "Color.h"
#include "assert.h"

#include "..\SpaceSim\Effect.h"

namespace tinyxml2
{
    XMLElement;
}

class Light
{
public:
	enum LightType
	{
	SPOT = 0,
	DIRECTIONAL,
	POint,
	};

	Light();
	Light(float range);
	Light(float range, const Vector3& position, const Color& diffuse, const Color& specular, LightType lighttype);
	~Light();

    void deserialise(const tinyxml2::XMLElement* element);

	//get and set
	const Color getDiffuse() const {return m_diffuse;}
	const Color getSpecular() const {return m_specular;}
	const Vector3 getPosition() const {return m_position;}
	float getRange() const {return m_range;}
	LightType getLightType() const {return m_lightType;}
	float getConstantAttenuation() const {return m_constantAttenuation;}
	float getLinearAttenuation() const {return m_linearAttenuation;}
	float getQuadraticAttenuation() const {return m_quadraticAttenuation;}
    float getInnerCone() { return m_innerCone; }
    float getOuterCone() { return m_outerCone; }
	float getFallOff() const {return m_fallOff;}
	const Vector3 getDirection() const {return m_direction;}
	void setDiffuse(const Color& diffuse) {m_diffuse = diffuse;}
	void setSpecular(const Color& specular) {m_specular = specular;}
	void setPosition(const Vector3& position) {m_position = position;}
	void setLightNumber(float range) {m_range = range;}
	void setLightType(const LightType type) {m_lightType = type;}
	void setConstantAttenuation(const float constant) {m_constantAttenuation = constant;}
	void setLinearAttenuation(const float linear) {m_linearAttenuation = linear;}
	void setQuadraticAttenuation(const float quadratic) {m_quadraticAttenuation = quadratic;}
	void setInnerCone(float innerCone) { m_innerCone = innerCone; }
    void setOuterCone(float outerCone) { m_outerCone = outerCone; }
	void setFallOff(float fallOff) { m_fallOff = fallOff;}
	void setDirection(const Vector3& direction) {m_direction = direction;}
    LightConstants getLightConstants() const;
    HASH_ELEMENT_DEFINITION
protected:
private:
	Vector3		m_position;
    Vector3		m_direction;
	Color		m_diffuse;
	Color		m_specular;
	LightType	m_lightType;
	float		m_range;
	float		m_constantAttenuation;
	float		m_linearAttenuation;
	float		m_quadraticAttenuation;
	float		m_innerCone;
    float		m_outerCone;
	float		m_fallOff;
};
#endif