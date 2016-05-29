#pragma once

#include <string>

class Paths
{
public:
    Paths();
    ~Paths() {}

    const std::string& getPath() const { return m_path; }
    const std::string& getModelPath() const { return m_modelpath; }
    const std::string& getTexturesPath() const { return m_texturespath; }
    const std::string& getLogPath() const { return m_logpath; }
    const std::string& getEffectShaderPath() const { return m_effectshaderpath; }
    const std::string& getScenePath() const { return m_scenesPath; }
    const std::string& getSettingsPath() const { return m_settingsPath; }
protected:
private:
    std::string m_path;
    std::string m_modelpath;
    std::string m_logpath;
    std::string m_texturespath;
    std::string m_effectshaderpath;
    std::string m_scenesPath;
    std::string m_settingsPath;
};
