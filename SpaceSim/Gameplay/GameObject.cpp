#include "GameObject.h"
#include "imgui.h"

void GameObject::OnDebugImgui()
{
    if (ImGui::TreeNode(m_name.c_str()))
    {
        ImGui::CollapsingHeader("World Matrix");
        ImGui::InputFloat4("World[0]", m_world[0]);
        ImGui::InputFloat4("World[1]", m_world[1]);
        ImGui::InputFloat4("World[2]", m_world[2]);
        ImGui::InputFloat4("World[3]", m_world[3]);

        OnDebugImguiInternal();
        ImGui::TreePop();
    }
}
