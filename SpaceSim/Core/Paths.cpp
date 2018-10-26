#include "Paths.h"
#include "StringOperations/StringHelperFunctions.h"

///-------------------------------------------------------------------------
///! @brief 
///-------------------------------------------------------------------------
Paths::Paths()
{
    m_path = std::filesystem::current_path();
    MSG_TRACE("path: %s", m_path.string().c_str());
    m_modelpath = m_path / "Models";
    m_texturespath = m_path / "Textures";
    m_logpath = m_path / "Logs";
    m_effectshaderpath = m_path / "Shaders";
    m_scenesPath = m_path / "Scenes";
    m_settingsPath = m_path / "Settings";
}