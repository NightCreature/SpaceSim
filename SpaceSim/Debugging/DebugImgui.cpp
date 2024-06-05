#include "Debugging/DebugImgui.h"

#include "Application/GameWindow.h"
#include "Core/Resource/RenderResource.h"
#include "Graphics/D3D12/DescriptorHeapManager.h"
#include "Graphics/D3D12/DeviceManagerD3D12.h"
#include "Graphics/D3D12/CommandQueue.h"


#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"


void DebugImgui::Init(Resource* resource)
{
    m_resource = resource;

    RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();


    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(renderResource.getGameWindow().getWindowHandle());

    DescriptorHeapManager& heapManager = renderResource.getDescriptorHeapManager();
    DescriptorHeap& imguiHeap = heapManager.GetImguiHeap();
    ImGui_ImplDX12_Init(renderResource.getDeviceManager().GetDevice(), 3,
        DXGI_FORMAT_R8G8B8A8_UNORM, imguiHeap.m_heap,
        imguiHeap.m_heap->GetCPUDescriptorHandleForHeapStart(),
        imguiHeap.m_heap->GetGPUDescriptorHandleForHeapStart());
}

void DebugImgui::Update(const Input& input)
{
    //Start the frame
    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //Call all the debug imgui functions we need
    for (auto& callback : m_imguiCallbacks)
    {
        if (callback.m_active)
        {
            callback.m_imguiCallback(input);
        }
    }
}

void DebugImgui::Render(CommandList& list)
{
    ImGui::Render();

    RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();
    DescriptorHeapManager& heapManager = renderResource.getDescriptorHeapManager();
    DescriptorHeap& imguiHeap = heapManager.GetImguiHeap();

    list.m_list->SetDescriptorHeaps(1, &imguiHeap.m_heap);
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), list.m_list);
}

void DebugImgui::Shutdown()
{
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
