#include "Paths.h"

#include <direct.h>

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
Paths::Paths()
{
    char buffer[1024];
    _getcwd(buffer, 1024);

    m_path = std::string(buffer);
    m_modelpath = m_path + "\\Models\\";
    m_texturespath = m_path + "\\Textures\\";
    m_logpath = m_path + "\\Logs\\";
    m_effectshaderpath = m_path + "\\Shaders\\";
    m_scenesPath = m_path + "\\Scenes\\";
    m_settingsPath = m_path + "\\Settings\\";
}