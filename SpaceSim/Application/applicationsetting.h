#pragma once

#include <..\SpaceSim\tinyxml2.h>

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
class ApplicationSetting
{
public:
	ApplicationSetting();
	~ApplicationSetting();

    void deserialise( const tinyxml2::XMLElement* element);

    const std::string& getApplicationName() const { return m_appName; }
    const std::string& getWindowName() const {return m_windowName; }
protected:
private:
    std::string m_appName;
    std::string m_windowName;
};