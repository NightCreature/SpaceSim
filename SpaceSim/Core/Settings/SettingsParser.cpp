#include "Core/Settings/SettingsParser.h"
#include "Core/Settings/SettingsManager.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/tinyxml2.h"
#include <iostream>

///-----------------------------------------------------------------------------
///! @brief   SettingsParser Constructor
///! @remark
///-----------------------------------------------------------------------------
SettingsParser::SettingsParser(SettingsManager* settingsMananger) :
m_settingsMananger(settingsMananger),
m_root(0)
{
}


///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
bool SettingsParser::loadFile( const std::string& fileName )
{
    m_fileName = fileName;
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(m_fileName.c_str()) != tinyxml2::XML_NO_ERROR)
    {
        MSG_TRACE_CHANNEL("SETTINGSPARSER", "Failed to load %s\nWith error: %d", fileName.c_str(), doc.ErrorID() )
        return false;
    }

    m_root = doc.FirstChildElement();

    //Go to the first child as the first node is the xml node
    tinyxml2::XMLNode* node = m_root->FirstChild();
    //If there are comments before the first actual node skip them
    node = skipComments(node);

    tinyxml2::XMLElement* element = node->ToElement();
    readDataSections(element, node);

    return true;
}

///-----------------------------------------------------------------------------
///! @brief   Cleanup the settings
///! @remark
///-----------------------------------------------------------------------------
void SettingsParser::cleanup()
{

}

///-----------------------------------------------------------------------------
///! @brief  Skip the comment nodes until we find another none comment node
///! @remark
///-----------------------------------------------------------------------------
tinyxml2::XMLNode* SettingsParser::skipComments( tinyxml2::XMLNode* node )
{
    while (node && node->ToComment())
    {
        MSG_TRACE_CHANNEL("SETTINGSPARSER", "COMMENT %s", node->ToComment()->Value() )
        node = node->NextSibling();
    }
    return node;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void SettingsParser::readStringValues( tinyxml2::XMLElement* element )
{
    tinyxml2::XMLElement* children = element->FirstChildElement();
    for (children; children; children = children->NextSiblingElement())
    {
        std::string settingName = std::string(children->Value());
        std::string text = children->GetText();

        m_settingsMananger->setSetting(new ISetting<std::string>(settingName, text));
    }
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void SettingsParser::readBooleanValues( tinyxml2::XMLElement* element )
{
    tinyxml2::XMLElement* children = element->FirstChildElement();
    for (children; children; children = children->NextSiblingElement())
    {
        std::string settingName = std::string(children->Value());
        std::string textValue = children->GetText();
        bool value = false;
        if (textValue == "true" || textValue == "1")
        {
            value = true;
        }

        m_settingsMananger->setSetting(new ISetting<bool>(settingName, value));
    }
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void SettingsParser::readintegerValues( tinyxml2::XMLElement* element )
{
    tinyxml2::XMLElement* children = element->FirstChildElement();
    for (children; children; children = children->NextSiblingElement())
    {
        std::string settingName = std::string(children->Value());
        std::string textValue = children->GetText();
        int value = strToint(textValue);

        m_settingsMananger->setSetting(new ISetting<int>(settingName, value));
    }
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void SettingsParser::readUnsignedintegerValues( tinyxml2::XMLElement* element )
{
    tinyxml2::XMLElement* children = element->FirstChildElement();
    for (children; children; children = children->NextSiblingElement())
    {
        std::string settingName = std::string(children->Value());
        std::string textValue = children->GetText();
        unsigned int value = (unsigned int)strToint(textValue);

        m_settingsMananger->setSetting(new ISetting<unsigned int>(settingName, value));
    }
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void SettingsParser::readfloatValues( tinyxml2::XMLElement* element )
{
    tinyxml2::XMLElement* children = element->FirstChildElement();
    for (children; children; children = children->NextSiblingElement())
    {
        std::string settingName = std::string(children->Value());
        std::string textValue = children->GetText();
        float value = strTofloat(textValue);

        m_settingsMananger->setSetting(new ISetting<float>(settingName, value));
    }
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void SettingsParser::readDoubleValues( tinyxml2::XMLElement* element )
{
    tinyxml2::XMLElement* children = element->FirstChildElement();
    for (children; children; children = children->NextSiblingElement())
    {
        std::string settingName = std::string(children->Value());
        std::string textValue = children->GetText();
        double value = strToDouble(textValue);

        m_settingsMananger->setSetting(new ISetting<double>(settingName, value));
    }
}

///-----------------------------------------------------------------------------
///! @brief   Read default data sections
///! @remark
///-----------------------------------------------------------------------------
tinyxml2::XMLElement* SettingsParser::readDataSections( tinyxml2::XMLElement* element, tinyxml2::XMLNode* node )
{
    element = node->ToElement();
    for( element; element; element = element->NextSiblingElement())
    {
        if (strICmp(element->Value(), "string"))
        {
            readStringValues(element);
        }
        else if (strICmp(element->Value(), "bool"))
        {
            readBooleanValues(element);
        }
        else if (strICmp(element->Value(), "int"))
        {
            readintegerValues(element);
        }
        else if (strICmp(element->Value(), "uint"))
        {
            readUnsignedintegerValues(element);
        }
        else if (strICmp(element->Value(), "float"))
        {
            readfloatValues(element);
        }
        else if (strICmp(element->Value(), "double"))
        {
            readDoubleValues(element);
        }
        else if (strICmp(element->Value(), "material") == 0 && element->FirstChildElement())
        {
            //MaterialSetting* materialSetting = new MaterialSetting();
            //materialSetting->deserialise(element->FirstChildElement());
            //m_settingsMananger->setSetting(materialSetting);
            MSG_TRACE_CHANNEL("SettingsReader", "TURNED OFF MATERIAL SETTINGS READING DUE TO INCLUDE ISSUES");
        }
        else if (strICmp(element->Value(), "vector") == 0 && element->FirstChildElement())
        {
            VectorSetting* vectorSetting = new VectorSetting();
            vectorSetting->deserialise(element->FirstChildElement());
            m_settingsMananger->setSetting(vectorSetting);
        }
    }
    
    return element;
}

