#include "SettingsManager.h"

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