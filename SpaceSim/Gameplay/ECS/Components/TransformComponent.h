#pragma once

#include "Math/matrix44.h"
#include "Gameplay/ECS/Component.h"
#include "Math/MathUtilityFunctions.h"
#include "Math/vector3.h"
#include "Math/vector4.h"

namespace ECS
{

struct TransformationComponent : public Component
{
    DECLARE_COMPONENT(TransformationComponent);

    Matrix44 m_transform;

    Vector3 m_position;
    Vector3 m_direction;
    Vector4 m_axis;
    Vector3 m_scaleVector;

    void Deserialise(const tinyxml2::XMLElement* element) override
    {
        /*<Translation x = "0" y = "0" z = "0" / >
        <Rotation x = "0" y = "0" z = "0" angle = "0" / > <!--If angle is zero assume we passed a direction-->
        <Scale x = "3000" y = "3000" z = "3000" / >*/

        Matrix44 transform;
        Matrix44 rotation;
        Matrix44 scaleMatrix;
        for (const tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement != 0; childElement = childElement->NextSiblingElement())
        {

            HashString elementName = HashString(childElement->Value());
            if (elementName == "position"_hashstring || elementName == "Translation"_hashstring)
            {
                
                m_position.deserialise(childElement);

                translate(transform, m_position);
            }
            else if (elementName == "direction"_hashstring)
            {
                
                m_direction.deserialise(childElement);
                m_direction.normalize();
                rotation = rotationFromDirection(m_direction);
            }
            else if (elementName == "rotation"_hashstring)
            {
                m_axis = math::Deserialise(childElement);

                Vector3 axis3 = Vector3(m_axis.x(), m_axis.y(), m_axis.z());
                axis3.normalize(); //make sure the angle is a unit vector

                rotate(rotation, axis3, m_axis.w());
            }
            else if (elementName == "scale"_hashstring)
            {
                m_scaleVector.deserialise(childElement);
                scale(scaleMatrix, m_scaleVector.x(), m_scaleVector.y(), m_scaleVector.z());
            }

        }

        m_transform = scaleMatrix * rotation * transform;

    }
};

}