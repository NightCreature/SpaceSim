#pragma once

#include "Core/Settings/Settings.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include <vector>

///-----------------------------------------------------------------------------
///! @brief  Todo Add comment
///! @remark
///-----------------------------------------------------------------------------
class SettingsManager
{
public:
    typedef std::vector<ISettingBase*> Settings;
    typedef std::vector<ISettingBase*>::iterator SettingsIterator;
    SettingsManager() {}
    ~SettingsManager() {}

    virtual void cleanup();

    const ISettingBase* getSetting(const std::string& SettingName) const 
    {
        int index = getSettingIndex(SettingName);
        if (index > -1)
        {
            return m_settings[index];
        }
        return 0;
    }

    //Helper function for when you actually know the type of the parameter you want to access
    template<class T>
    const ISetting<T>* getSetting(const std::string& settingName) const
    {
        return (ISetting<T>*)getSetting(settingName);
    }

    void setSetting(ISettingBase* value)
    {
        int index = getSettingIndex(value->getSettingName());
        if (index > -1)
        {
            if (m_settings[index] != 0)
            {
                MSG_TRACE_CHANNEL("SETTINGS", "warning this setting already has a value setting in question is: %s", m_settings[index]->getSettingName().c_str() )
                ISettingBase* setting = m_settings[index];
                delete setting;
            }
            m_settings[index] = value;
        }
        else
        {
            m_settings.push_back(value);
        }
    }
protected:
private:
    int getSettingIndex(const std::string& settingName) const 
    {
        unsigned int hashedSettingName = hashString(settingName);
        for (int counter = 0; counter < (int)m_settings.size(); ++counter)
        {
            if (m_settings[counter]->getHashValue() == hashedSettingName)
            {
                return counter;
            }
        }

        return -1;
    }

	Settings    m_settings;
};