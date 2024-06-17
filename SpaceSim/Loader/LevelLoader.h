#pragma once
#include <filesystem>
#include <Graphics/material.h>
#include <Graphics/light.h>

class Resource;
class Level;
namespace MessageSystem { class MessageObserver; }

class LevelLoader
{
public:
    LevelLoader(Resource* resource) : m_resource(resource) {}
    ~LevelLoader() = default;

    Level LoadLevel(const std::filesystem::path& filename, MessageSystem::MessageObserver& messageObservers);
    Level LoadLevel(const std::filesystem::path& filename, MessageSystem::MessageObserver& messageObservers, std::true_type /*archive*/);
    void SaveLevel(const std::filesystem::path& filename, const Level& level);
    void SaveLevel(const std::filesystem::path& filename, const Level& level, std::true_type /*archive*/);
private:
    Resource* m_resource;
};