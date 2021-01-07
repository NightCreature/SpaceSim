#pragma once

#include <filesystem>
#include <string>

//This class should generate mount points instead
class Paths
{
public:
    Paths();
    ~Paths() {}

    const std::filesystem::path& getPath() const { return m_path; }
    const std::filesystem::path& getModelPath() const { return m_modelpath; }
    const std::filesystem::path& getTexturesPath() const { return m_texturespath; }
    const std::filesystem::path& getLogPath() const { return m_logpath; }
    const std::filesystem::path& getEffectShaderPath() const { return m_effectshaderpath; }
    const std::filesystem::path& getScenePath() const { return m_scenesPath; }
    const std::filesystem::path& getSettingsPath() const { return m_settingsPath; }

    const std::string getPathStr() const { return m_path.string(); }
    const std::string getModelPathStr() const { return m_modelpath.string(); }
    const std::string getTexturesPathStr() const { return m_texturespath.string(); }
    const std::string getLogPathStr() const { return m_logpath.string(); }
    const std::string getEffectShaderPathStr() const { return m_effectshaderpath.string(); }
    const std::string getScenePathStr() const { return m_scenesPath.string(); }
    const std::string getSettingsPathStr() const { return m_settingsPath.string(); }
    
protected:
private:
    std::filesystem::path m_path;
    std::filesystem::path m_modelpath;
    std::filesystem::path m_logpath;
    std::filesystem::path m_texturespath;
    std::filesystem::path m_effectshaderpath;
    std::filesystem::path m_scenesPath;
    std::filesystem::path m_settingsPath;
};
