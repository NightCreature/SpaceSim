#pragma once

#include "Graphics/D3D12/DescriptorHeapManager.h"

#include <functional>
#include <string>
#include <vector>

class Resource;
class Input;
struct CommandList;

class DebugImgui;

void RegisterDebugImguiCallbacks(Resource* resource, DebugImgui* debugImgui);

class DebugImgui
{
public:
    DebugImgui() {}
    ~DebugImgui() {}

    void Init(Resource* resource);
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
    Resource* m_resource = nullptr;
    DescriptorHeap m_heap;

    void CreateImguiMenu();
};
