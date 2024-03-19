#include "Graphics/D3D12/DeviceManagerD3D12.h"

#include "Core/StringOperations/StringHelperFunctions.h"
#include <dxgidebug.h>
#include "Core/Resource/RenderResource.h"
#include "D3D12X.h"
#include <sstream>

extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 608; }

extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = ".\\D3D12\\"; } //Path to where we can find D3D12Core.dll

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void DeviceManager::cleanup()
{
    for (auto* resource : m_backBufferRenderTargets)
    {
        if (resource != nullptr)
        {
            resource->Release();
        }
    }
    
    if (m_depthStencil != nullptr)
    {
        m_depthStencil->Release();
    }

    if (m_swapChain != nullptr)
    {
        m_swapChain->Release();
    }
   
    m_dxgiFactory->Release();
    m_device->Release();

//#if DEBUG
    IDXGIDebug1* pDebug = nullptr;
    if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&pDebug))))
    {
        pDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
        pDebug->Release();
    }
//#endif
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool DeviceManager::createDevice()
{
    if (!InitialiseDebugLayers())
    {
        //This is not a hard error
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "No D3D debug layers!!!!!");
    }

    if (!InitialiseDXGIFactory())
    {
        return false;
    }

    //Initialise and enumerate the device
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory7), (void**)(&m_dxgiFactory));
    if (hr == S_OK)
    {
        IDXGIAdapter* adapter = EnumerateAdapters();
        
        if (adapter != nullptr)
        {
            MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Creating Device");
            //Figure out what the highest feature level is we support

            D3D_FEATURE_LEVEL featureLevels[] = {
                    D3D_FEATURE_LEVEL_12_1,
                    D3D_FEATURE_LEVEL_12_0,
                    D3D_FEATURE_LEVEL_11_1,
                    D3D_FEATURE_LEVEL_11_0,
                    D3D_FEATURE_LEVEL_10_1,
                    D3D_FEATURE_LEVEL_10_0,
                    D3D_FEATURE_LEVEL_9_3,
                    D3D_FEATURE_LEVEL_9_2,
                    D3D_FEATURE_LEVEL_9_1,
                    D3D_FEATURE_LEVEL_1_0_CORE,
            };
            for (size_t counter = 0; counter < 10; ++counter)
            {

                hr = D3D12CreateDevice(adapter, featureLevels[counter], __uuidof(ID3D12Device), nullptr);
                if (hr == S_FALSE || hr == S_OK)
                {
                    m_featureLevel = featureLevels[counter];
                    break;
                }
            }

            //m_featureLevel = D3D_FEATURE_LEVEL_12_1;

            if (m_featureLevel == 0)
            {
                MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to select a feature level");
                return false;
            }

            MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Selected Feature level: 0x%x", m_featureLevel);

            hr = D3D12CreateDevice(adapter, m_featureLevel, IID_PPV_ARGS(&m_device));
            if (FAILED(hr))
            {
                MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create a D3D device with error code: 0x%x %s", hr, getLastErrorMessage(hr))
                return false;
            }

            MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Device Created");

#ifdef _DEBUG
            m_device->SetPrivateData(WKPDID_D3DDebugObjectName, 13, "Render Device");
#endif
            adapter->Release();

            if (!CheckFeatures())
            {
                MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Device Doesn't support required features");
                return false;
            }

            return true;
        }
    }

    return false;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
IDXGIAdapter* DeviceManager::EnumerateAdapters()
{
    IDXGIAdapter* adapter = nullptr;

    adapter = nullptr;
    //Ask for the high performance GPU, this should be the dedicated GPU in a laptop
    m_dxgiFactory->EnumAdapterByGpuPreference((UINT)0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Graphics Adapter number: %d", 0);
    if (adapter != nullptr)
    {
        DXGI_ADAPTER_DESC adapterDesc;
        adapter->GetDesc(&adapterDesc);
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Graphics Adapter vendor id: %x", adapterDesc.VendorId);
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Graphics Adapter device id: %x", adapterDesc.DeviceId);
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Graphics Adapter subsytem id: %x", adapterDesc.SubSysId);
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Graphics Adapter revision: %d", adapterDesc.Revision);
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Graphics Adapter Dedicated VRAM: %llu MiB", adapterDesc.DedicatedVideoMemory / (1024 * 1024));
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Graphics Adapter Dedicated RAM: %llu MiB", adapterDesc.DedicatedSystemMemory / (1024 * 1024));
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Graphics Adapter Shared RAM: %llu MiB", adapterDesc.SharedSystemMemory / (1024 * 1024));
        std::string str;
        convertToCString(adapterDesc.Description, str);
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "description: %s", str.c_str());
    }

    return adapter;
}


///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
ID3D12Resource* DeviceManager::GetCurrentBackBuffer()
{
    return m_backBufferRenderTargets[m_currentBackBufferIndex];
}

///-----------------------------------------------------------------------------
///! @brief Implment feature checks here if we need them
///! @remark
///-----------------------------------------------------------------------------
bool DeviceManager::CheckFeatures()
{
    D3D12_FEATURE_DATA_D3D12_OPTIONS featureSupport = {};
    m_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &featureSupport, sizeof(featureSupport));

    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Conservative Rasterisation Tier: %d", featureSupport.ConservativeRasterizationTier);
    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Cross Adapter Row Major Textures: %s", featureSupport.CrossAdapterRowMajorTextureSupported ? "true" : "false");
    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Cross Node sharing tier: %d", featureSupport.CrossNodeSharingTier);
    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Double Precision Float Shader Ops: %s", featureSupport.DoublePrecisionFloatShaderOps ? "true" : "false");
    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Max GPU Virtual Adress bits: %d", featureSupport.MaxGPUVirtualAddressBitsPerResource);
    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Min Precision Support (10, 16 bits): %d", featureSupport.MinPrecisionSupport);
    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Output Merger Logic: %s", featureSupport.OutputMergerLogicOp ? "true" : "false");
    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "PS Specified Stencil Reference Support: %s", featureSupport.PSSpecifiedStencilRefSupported ? "true" : "false");
    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Resource Binding Tier: %d", featureSupport.ResourceBindingTier);

    D3D12_FEATURE_DATA_ARCHITECTURE architecture;
    architecture.NodeIndex = 0;
    m_device->CheckFeatureSupport(D3D12_FEATURE_ARCHITECTURE, &architecture, sizeof(architecture));
    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Type of GPU: %s", architecture.UMA ? "APU" : "DGPU");

    //Initialise this with the highest possible value, if its supported it will return that value or the highest the device does support
    D3D12_FEATURE_DATA_SHADER_MODEL shaderModel;
    shaderModel.HighestShaderModel = D3D_SHADER_MODEL_6_6;
    m_device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &shaderModel, sizeof(shaderModel));

    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Highest Shader Model supported: 0x%X", shaderModel.HighestShaderModel);
    
    D3D12_FEATURE_DATA_D3D12_OPTIONS5 featureSupport5{};
    m_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &featureSupport5, sizeof(featureSupport5));
    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Raytracing tier: %d", featureSupport5.RaytracingTier);
    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Render Passes Tier: %d", featureSupport5.RenderPassesTier);
    MSG_TRACE_CHANNEL("DeviceManagerD3D12", "SRV Only tiled resources tier 3: %s", featureSupport5.SRVOnlyTiledResourceTier3 ? "true" : "false");

    return true;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool DeviceManager::createSwapChain(HWND windowHandle, int windowWidth, int windowHeight)
{
    auto writableResource = RenderResourceHelper(m_resource).getWriteableResource();

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    auto& commandQueueManager = writableResource.getCommandQueueManager();
    m_swapChainCommandQueueIndex = commandQueueManager.CreateCommandQueue();
    auto& commandQueue = commandQueueManager.GetCommandQueue(m_swapChainCommandQueueIndex);

    m_swapChainCommandListIndex = commandQueue.CreateCommandList(); 

    // Describe and create the swap chain.
    m_swapChainDescriptor = {};
    m_swapChainDescriptor.BufferCount = static_cast<UINT>(m_frameCount);
    m_swapChainDescriptor.Width = windowWidth;
    m_swapChainDescriptor.Height = windowHeight;
    m_swapChainDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    m_swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    m_swapChainDescriptor.BufferCount = static_cast<UINT>(m_frameCount);
    m_swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    m_swapChainDescriptor.SampleDesc.Count = 1;
    m_swapChainDescriptor.Stereo = false;

    // Swap chain needs the queue so that it can force a flush on it.
    IDXGISwapChain1* tempSwapChain = nullptr;
    HRESULT hr = m_dxgiFactory->CreateSwapChainForHwnd(commandQueue.m_queue, windowHandle, &m_swapChainDescriptor, nullptr, nullptr, &tempSwapChain);
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create a swap chain with error: 0x%x, %s", hr, getLastErrorMessage(hr));
        return false;
    }

    m_swapChain = static_cast<IDXGISwapChain4*>(tempSwapChain);

    m_currentBackBufferIndex = m_swapChain->GetCurrentBackBufferIndex();

    //Create the heap for the back buffers we still have to attach the resources
    m_renderTargetViewDescriptorHeap = writableResource.getDescriptorHeapManager().CreateDescriptorHeap(m_frameCount, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, false, false);

    
    //Create actual resources for the backbuffers
    //Grab the size of a single descriptor for this heap
    // Create frame resources.

    m_backBufferRenderTargets.resize(m_frameCount);//Rerseve space for the back bufffers
    // Create a RTV for each frame.
    for (size_t counter = 0; counter < m_frameCount; ++counter)
    {
        hr = m_swapChain->GetBuffer(static_cast<UINT>(counter), IID_PPV_ARGS(&m_backBufferRenderTargets[counter]));
        if (hr != S_OK)
        {
            MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to grab the backbuffer for the swapchain with error: 0x%x, %s", hr, getLastErrorMessage(hr));
            return false;
        }
        size_t descriptorIndex = DescriptorHeap::invalidDescriptorIndex;
        D3D12_CPU_DESCRIPTOR_HANDLE  rtvHandle = m_renderTargetViewDescriptorHeap.GetCPUDescriptorHandle(descriptorIndex);
        m_device->CreateRenderTargetView(m_backBufferRenderTargets[counter], nullptr, rtvHandle);

#ifdef _DEBUG
        std::wstringstream str;
        str << L"Back Buffer Render Target" << counter;
        m_backBufferRenderTargets[counter]->SetName(str.str().c_str());
#endif
    }


    //Create the Depth Stencil heap
    m_depthStencilDescriptorHeap = writableResource.getDescriptorHeapManager().CreateDescriptorHeap(1, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, false, false);
    

    // Create the depth stencil view.
    {
        D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
        depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
        depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

        D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
        depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
        depthOptimizedClearValue.DepthStencil.Depth = 0.0f;
        depthOptimizedClearValue.DepthStencil.Stencil = 0;

        D3D12_RESOURCE_DESC depthStencilDescriptor;
        depthStencilDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
        depthStencilDescriptor.Width = windowWidth;
        depthStencilDescriptor.Height = windowHeight;
        depthStencilDescriptor.DepthOrArraySize = 1;
        depthStencilDescriptor.MipLevels = 0;
        depthStencilDescriptor.SampleDesc.Count = 1;
        depthStencilDescriptor.SampleDesc.Quality = 0;
        depthStencilDescriptor.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
        depthStencilDescriptor.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
        depthStencilDescriptor.Alignment = 0;
        depthStencilDescriptor.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

        D3D12_HEAP_PROPERTIES heapProperties;
        heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
        heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        heapProperties.CreationNodeMask = 1;
        heapProperties.VisibleNodeMask = 1;

        auto depthDescritpionD3Dx = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, windowWidth, windowHeight, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

        hr = m_device->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &depthDescritpionD3Dx, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthOptimizedClearValue, IID_PPV_ARGS(&m_depthStencil));
        if (hr != S_OK)
        {
            MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create depth stencil resource with error: 0x%x, %s", hr, getLastErrorMessage(hr));
            return false;
        }

#ifdef _DEBUG
        std::wstringstream str;
        str << L"Depth Stencil";
        m_depthStencil->SetName(str.str().c_str());
#endif


        
        m_device->CreateDepthStencilView(m_depthStencil, &depthStencilDesc, m_depthStencilDescriptorHeap.GetCPUDescriptorHandle(depthStencilDescriptorIndex));
    }

    return true;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
D3D12_CPU_DESCRIPTOR_HANDLE DeviceManager::GetDepthStencilHandle()
{
    return m_depthStencilDescriptorHeap.GetCPUDescriptorHandle(depthStencilDescriptorIndex);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
D3D12_CPU_DESCRIPTOR_HANDLE DeviceManager::GetCurrentBackBufferRTVHandle()
{
    return m_renderTargetViewDescriptorHeap.GetCPUDescriptorHandle(m_currentBackBufferIndex);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool DeviceManager::InitialiseDebugLayers()
{
#if defined(_DEBUG)
    // Enable the D3D12 debug layer.
    HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugInterface));
    if (hr != S_OK || m_debugInterface == nullptr)
    {
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create the debug interface with error: 0x%x, %s", hr, getLastErrorMessage(hr));
        return false;
    }

    hr = m_debugInterface->QueryInterface(IID_PPV_ARGS(&m_debug1Interface));
	if (hr != S_OK || m_debug1Interface == nullptr)
	{
		MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create the debug1 interface with error: 0x%x, %s", hr, getLastErrorMessage(hr));
		return false;
	}

    m_debug1Interface->SetEnableGPUBasedValidation(true);

    m_debugInterface->EnableDebugLayer();
#endif

    return true;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool DeviceManager::InitialiseDXGIFactory()
{

    HRESULT hr  = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create the DXGI interface with error: 0x%x, %s", hr, getLastErrorMessage(hr));
        return false;
    }

    return true;
}