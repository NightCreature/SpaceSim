#pragma once
#include "Core/Settings/Settings.h"
#include <map>
#include <string>
#include "Core/Settings/SettingsManager.h"

namespace tinyxml2
{
    class XMLElement;
    class XMLNode;
}

class SettingsParser
{
public:
    SettingsParser(SettingsManager* settingsMananger);
    ~SettingsParser() {}

    virtual bool  loadFile(const std::string& fileName);
    tinyxml2::XMLElement* readDataSections( tinyxml2::XMLElement* element, tinyxml2::XMLNode* node );
    void cleanup();
protected:
private:
    tinyxml2::XMLNode* skipComments( tinyxml2::XMLNode* node );
    void readStringValues( tinyxml2::XMLElement* element );
    void readBooleanValues( tinyxml2::XMLElement* element );
    void readintegerValues( tinyxml2::XMLElement* element );
    void readUnsignedintegerValues( tinyxml2::XMLElement* element );
    void readfloatValues( tinyxml2::XMLElement* element );
    void readDoubleValues( tinyxml2::XMLElement* element );

    SettingsManager* m_settingsMananger;
    tinyxml2::XMLElement* m_root;
    std::string m_fileName;
};
