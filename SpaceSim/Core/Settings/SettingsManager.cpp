#include "SettingsManager.h"

#include "Core/Serialization/Archive.h"

///-----------------------------------------------------------------------------
///! @brief  Todo Add comment
///! @remark
///-----------------------------------------------------------------------------
void SettingsManager::cleanup()
{

    if (!m_settings.empty())
    {
        for (SettingsIterator it = m_settings.begin(); it != m_settings.end(); ++it)
        {
            ISettingBase* value = *it;
            delete value;
        }

        m_settings.clear();
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void SettingsManager::SaveSettings(const std::filesystem::path& settingsFilePath)
{
    Archive settingsArchive;
    settingsArchive.Open(settingsFilePath);

    settingsArchive.Write(m_settings);

    settingsArchive.Close();
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void SettingsManager::LoadSettings(const std::filesystem::path& settingsFilePath)
{
    Archive settingsArchive;
    settingsArchive.Open(settingsFilePath);

    Settings settings;
    settingsArchive.Read(settings);

    settingsArchive.Close();
}
