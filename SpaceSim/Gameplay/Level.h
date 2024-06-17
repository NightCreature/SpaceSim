#pragma once

#include "Graphics/material.h"
#include "Graphics/light.h"

#include <vector>
#include "Core/MessageSystem/Messages.h"

class MessageSystem::Message;

// Container class for level data and editor functionality
// This class is responsible for holding references to all objects in a level
class Level : public MessageSystem::IMessageDispatcher
{
public:
    Level() = default;
    ~Level() = default;

    bool IsValid() const { return !m_materials.empty() || !m_lights.empty() || !m_entitiesIds.empty(); }

    // Add a material to the level
    void AddMaterial(const HashString& materialName) { m_materials.push_back(materialName); }
    // Add a light to the level
    void AddLight(const HashString& lightName) { m_lights.push_back(lightName); }
    // Add an entity to the level
    void AddEntity(const std::string& name, size_t entityId) { m_entityNames.push_back(name); m_entitiesIds.push_back(entityId); }
    // Get the materials in the level
    const std::vector<HashString>& GetMaterials() const { return m_materials; }
    // Get the lights in the level
    const std::vector<HashString>& GetLights() const { return m_lights; }
    // Get the entities in the level
    const std::vector<size_t>& GetEntities() const { return m_entitiesIds; }

    bool ProcessDataAndRequestResources();

    void OnDebugImGui();

    void dispatchMessage(const MessageSystem::Message& msg) override;
private:
    std::vector<HashString> m_materials;
    std::vector<HashString> m_lights;
    std::vector<size_t> m_entitiesIds;
    std::vector<std::string> m_entityNames;
};
