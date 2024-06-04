#include "Core/Settings/Settings.h"
#include "Core/tinyxml2.h"
#include "Core/StringOperations/StringHelperFunctions.h"

//IMPLEMENT_SERIALIZATION_OBJECT(ISettingBase);
//IMPLEMENT_SERIALIZATION_OBJECT(DeserialisableSetting);
//IMPLEMENT_SERIALIZATION_OBJECT(RenderSetting);
//IMPLEMENT_SERIALIZATION_OBJECT(VectorSetting);

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void DeserialisableSetting::deserialise( const tinyxml2::XMLElement* element)
{
    m_settingName = element->Attribute("name");
    m_type = ISettingBase::SettingType::eUserType;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void RenderSetting::deserialise( const tinyxml2::XMLElement* element)
{
    DeserialisableSetting::deserialise(element);

    for (const tinyxml2::XMLAttribute* attribute = element->FirstAttribute(); attribute; attribute = attribute->Next())
    {
        if (strICmp(attribute->Name(), "use_cg") == 0)
        {
            m_useCG = attribute->BoolValue();
        }
        else if (strICmp(attribute->Name(), "renderer") == 0)
        {
            m_rendererType = (RendererType)attribute->IntValue();
        }
    }

    for (const tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement() )
    {
        if (strICmp(childElement->Value(), "WindowSettings") == 0)
        {
            for (const tinyxml2::XMLAttribute* attribute = childElement->FirstAttribute(); attribute; attribute = attribute->Next())
            {
                if (strICmp(attribute->Name(), "width") == 0)
                {
                    m_resolutionWidth = attribute->IntValue();
                }
                else if (strICmp(attribute->Name(), "height") == 0)
                {
                    m_resolutionHeight = static_cast<unsigned int>(attribute->IntValue());
                }
                else if (strICmp(attribute->Name(), "window_name") == 0)
                {
                    m_windowName = attribute->Value();
                }
            }
        }
    }
}

///-----------------------------------------------------------------------------
///! @brief   Deserialise a material from an xml node
///! @remark
///-----------------------------------------------------------------------------
//void MaterialSetting::deserialise( const tinyxml2::XMLElement* element)
//{
//    DeserialisableSetting::deserialise(element);
//
//    for (const tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement() )
//    {
//        if (strICmp(childElement->Value(), "Color") == 0)
//        {
//            Color color;
//            for (const tinyxml2::XMLAttribute* attribute = childElement->FirstAttribute(); attribute; attribute = attribute->Next())
//            {
//                if ( strICmp(attribute->Name(), "r") == 0 )
//                {
//                    color.setRed((float)attribute->DoubleValue());
//                }
//                else if ( strICmp(attribute->Name(), "g") == 0 )
//                {
//                    color.setGreen((float)attribute->DoubleValue());
//                }
//                else if ( strICmp(attribute->Name(), "b") == 0 )
//                {
//                    color.setBlue((float)attribute->DoubleValue());
//                }
//                else if ( strICmp(attribute->Name(), "a") == 0 )
//                {
//                    color.setAlpha((float)attribute->DoubleValue());
//                }
//            }
//
//            const char* materialPart = childElement->Attribute("material_part");
//            if (strICmp(materialPart, "ambient"))
//            {
//                m_material.setAmbient(color);
//            }
//            else if (strICmp(materialPart, "diffuse"))
//            {
//                m_material.setDiffuse(color);
//            }
//            else if (strICmp(materialPart, "specular"))
//            {
//                m_material.setSpecular(color);
//            }
//            else if (strICmp(materialPart, "emissive"))
//            {
//                m_material.setEmissive(color);
//            }
//        }
//        else if (strICmp(childElement->Value(), "Shininess") == 0)
//        {
//            //Deserialise the single float value of the material here
//            m_material.setShininess(strTofloat(childElement->GetText()));
//        }
//    }
//}
///-----------------------------------------------------------------------------
///! @brief   deserialise a vector in from xml
///! @remark
///-----------------------------------------------------------------------------
void VectorSetting::deserialise( const tinyxml2::XMLElement* element)
{
    DeserialisableSetting::deserialise(element);
	m_numberElements = 0;
    const tinyxml2::XMLAttribute* attribute = element->FindAttribute("type");
    if (attribute)
    {
        m_numberElements = attribute->IntValue();
    }
    
    float array[] = {0.0f, 0.0f, 0.0f, 0.0f};
    attribute = element->FirstAttribute();
    for (int counter = 0; counter < m_numberElements ; ++counter)
    {
        attribute = attribute->Next();
        if (attribute != nullptr)
        {
            array[counter] = (float)attribute->DoubleValue();
        }
    }

    m_vector = Vector4(array);
}
