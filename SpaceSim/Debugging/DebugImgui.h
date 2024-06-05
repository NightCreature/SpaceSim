#pragma once

#include <vector>
#include <functional>

class Resource;
class Input;
struct CommandList;

class DebugImgui
{
public:
    DebugImgui() {}
    ~DebugImgui() {}

    void Init(Resource* resource);
    void Update(const Input& input);
    void Render(CommandList& list);
    void Shutdown();

    void RegisterImguiCallback(auto imguiFunc)
    {
        ImguiData imguiData;
        imguiData.m_imguiCallback = imguiFunc;
        m_imguiCallbacks.push_back(imguiData);
    }
private:
    struct ImguiData
    {
        ImguiData() : m_imguiCallback(nullptr) {}
        std::function<void(const Input&)> m_imguiCallback;

        bool m_active = true;
    };

    std::vector<ImguiData> m_imguiCallbacks;
    Resource* m_resource = nullptr;

};