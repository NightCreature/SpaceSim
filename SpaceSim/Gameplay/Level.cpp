#include "Gameplay/Level.h"

#include "imgui.h"
#include "Core/MessageSystem/GameMessages.h"

bool Level::ProcessDataAndRequestResources()
{
    //From here for each non entity object in the level we need to request resources
    //auto message = CREATERENDERRESOURCEMESSAGE<Light>;
    //message.m_data = m_lights;

    return true;
}

void Level::OnDebugImGui()
{
    if (ImGui::Begin("Level"))
    {
        if (ImGui::CollapsingHeader("Lights"))
        {
            for (size_t i = 0; i < m_lights.size(); ++i)
            {
                m_lights[i].OnDebugImgui();
            }
        }

        if (ImGui::CollapsingHeader("Entities"))
        {
            for (size_t i = 0; i < m_entitiesIds.size(); ++i)
            {
                ImGui::Text("Entity %d", i);
                ImGui::Text("ID: %d", m_entitiesIds[i]);
                ImGui::Text("Name: %s", m_entityNames[i].c_str());
            }
        }
    }
    ImGui::End();
}

void Level::dispatchMessage(const MessageSystem::Message& msg)
{
    
}

