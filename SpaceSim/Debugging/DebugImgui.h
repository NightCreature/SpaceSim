#pragma once

#include "Graphics/D3D12/DescriptorHeapManager.h"

#include <functional>
#include <string>
#include <vector>

class Resource;
class RenderResource;
class GameResource;
class Input;
struct CommandList;

class DebugImgui;

void RegisterDebugImguiCallbacks(RenderResource* renderResource, GameResource* gameResource, DebugImgui* debugImgui);

class DebugImgui
{
public:
    DebugImgui() {}
    ~DebugImgui() {}

    void Init(RenderResource* renderResource, GameResource* gameResource);
    void Update();
    void Render(CommandList& list);
    void Shutdown();

    void RegisterImguiCallback(auto imguiFunc, const std::string_view& name)
    {
        ImguiData imguiData;
        imguiData.m_imguiCallback = imguiFunc;
        imguiData.m_name = name;
        m_imguiCallbacks.push_back(imguiData);
    }
private:
    struct ImguiData
    {
        ImguiData() : m_imguiCallback(nullptr) {}
        std::function<void()> m_imguiCallback;
        std::string m_name;

        bool m_active = false;
    };

    std::vector<ImguiData> m_imguiCallbacks;
    GameResource* m_gameResource = nullptr;
    RenderResource* m_renderResource = nullptr;
    DescriptorHeap m_heap;

    void CreateImguiMenu();
};
