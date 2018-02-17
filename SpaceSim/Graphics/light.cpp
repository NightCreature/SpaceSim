#include "Graphics/light.h"
#include <Core/tinyxml2.h>


HASH_ELEMENT_IMPLEMENTATION(Light) 

Light::Light()
{
	m_range = 1000.0;
	m_position = Vector3(0.0, 0.0, 0.0);
	m_diffuse = Color::black();
	m_specular = Color::black();
	m_lightType = DIRECTIONAL;
}

Light::Light(float range)
{
	m_range = range;
	m_lightType = DIRECTIONAL;
}

Light::Light(float range, const Vector3& position, const Color& diffuse, const Color& specular, LightType lighttype)
{
	m_range = range;
	m_diffuse = diffuse;
	m_specular = specular;
	m_lightType = lighttype;
	m_position = position;
}

Light::~Light()
{
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Light::deserialise( const tinyxml2::XMLElement* element )
{
    const tinyxml2::XMLAttribute* attribute = element->FindAttribute("type");
    if (attribute != nullptr)
    {
        m_lightType = (LightType)attribute->IntValue();
    }
    attribute = element->FindAttribute("att");
    if (attribute != nullptr)
    {
        m_constantAttenuation = attribute->FloatValue();
    }
    attribute = element->FindAttribute("lin_att");
    if (attribute != nullptr)
    {
        m_linearAttenuation = attribute->FloatValue();
    }
    attribute = element->FindAttribute("quad_att");
    if (attribute != nullptr)
    {
        m_quadraticAttenuation = attribute->FloatValue();
    }
    attribute = element->FindAttribute("inner_cone");
    if (attribute != nullptr)
    {
        m_innerCone = attribute->FloatValue();
    }
    attribute = element->FindAttribute("outer_cone");
    if (attribute != nullptr)
    {
        m_outerCone = attribute->FloatValue();
    }
    attribute = element->FindAttribute("exponent");
    if (attribute != nullptr)
    {
        m_fallOff = attribute->FloatValue();
    }
    attribute = element->FindAttribute("range");
    if (attribute != nullptr)
    {
        m_range = attribute->FloatValue();
    }

    int vector3Counter = 0;
    int colorCounter = 0;
    for (element = element->FirstChildElement(); element != 0; element = element->NextSiblingElement())
    {
        unsigned int elementHash = hashString(element->Value());
        if (elementHash == Vector3::m_hash)
        {
            if (vector3Counter == 0)
            {
                ++vector3Counter;
                m_position.deserialise(element);
            }
            else if (vector3Counter == 1)
            {
                ++vector3Counter;
                m_direction.deserialise(element);
            }
        }
        else if(elementHash == Color::m_hash)
        {
            if (colorCounter == 0)
            {
                ++colorCounter;
                m_diffuse.deserialise(element);
            }
            else if (colorCounter == 1)
            {
                ++colorCounter;
                m_specular.deserialise(element);
            }
        }
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
LightConstants Light::getLightConstants() const
{
    LightConstants constants;
    constants.m_position[0] = m_position.x();
    constants.m_position[1] = m_position.y();
    constants.m_position[2] = m_position.z();
    constants.m_position[3] = m_innerCone;
    constants.m_direction[0] = m_direction.x();
    constants.m_direction[1] = m_direction.y();
    constants.m_direction[2] = m_direction.z();
    constants.m_direction[3] = m_outerCone;
    constants.m_diffuse[3] = m_diffuse.a();
    constants.m_diffuse[0] = m_diffuse.r();
    constants.m_diffuse[1] = m_diffuse.g();
    constants.m_diffuse[2] = m_diffuse.b();
    constants.m_specular[3] = m_specular.a();
    constants.m_specular[0] = m_specular.r();
    constants.m_specular[1] = m_specular.g();
    constants.m_specular[2] = m_specular.b();
    constants.m_attenuationFallOff[0] = m_constantAttenuation;
    constants.m_attenuationFallOff[1] = m_linearAttenuation;
    constants.m_attenuationFallOff[2] = m_quadraticAttenuation;
    constants.m_attenuationFallOff[3] = m_fallOff;
    constants.m_range[0] = m_range;
    constants.m_range[1] = 0.0f;
    constants.m_range[2] = 0.0f;
    constants.m_range[3] = 0.0f;

    return constants;
}
