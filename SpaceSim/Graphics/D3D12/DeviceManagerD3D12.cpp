#include "Graphics/D3D12/DeviceManagerD3D12.h"

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool DeviceManagerD3D12::createDevice()
{
    if (!InitialiseDebugLayers())
    {
        //This is not a hard error
        MSG_TRACE_CHANNEL("No D3D debug layers!!!!!");
    }

    if (!InitialiseDXGIFactory())
    {
        return false;
    }

    //Initialise and enumerate the device
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&m_dxgiFactory));
    if (hr == S_OK)
    {
        IDXGIAdapter1* adapter = EnumerateAdapters();
        
        if (adapter != nullptr)
        {
            MSG_TRACE_CHANNEL("DEVICEMANAGER", "Creating Device");
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
                    D3D_FEATURE_LEVEL_1_0_CORE
            };
            for (auto featureLevel : featureLevels)
            {

                hr = D3D12CreateDevice(adapter, featureLevel, __uuidof(ID3D12Device), nullptr);
                if (hr == S_OK)
                {
                    m_featureLevel = featureLevel;
                }
            }

            if (m_featureLevel == 0)
            {
                MSG_TRACE_CHANNEL("DEVICEMANAGER", "Failed to select a feature level");
                return false;
            }

            MSG_TRACE_CHANNEL("DEVICEMANAGER", "Selected Feature level: 0x%x", featureLevel);

            hr = D3D12CreateDevice(adapter, m_featureLevel, __uuidof(ID3D12Device), &m_device);
            if (FAILED(hr))
            {
                MSG_TRACE_CHANNEL("DEVICEMANAGER", "Failed to create a D3D device with error code: 0x%x %s", hr, getLastErrorMessage(hr))
                    return false;
            }

#ifdef _DEBUG
            m_device->SetPrivateData(WKPDID_D3DDebugObjectName, 13, "Render Device");
#endif
            adapter->Release();

            //Do some driver feautre checking
            D3D11_FEATURE_DATA_THREADING threadingSupport;
            ZeroMemory((void*)&threadingSupport, sizeof(D3D11_FEATURE_DATA_THREADING));
            m_device->CheckFeatureSupport(D3D11_FEATURE_THREADING, (void*)&threadingSupport, sizeof(D3D11_FEATURE_DATA_THREADING));

            return true;
        }
    }

    return false;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
IDXGIAdapter* DeviceManagerD3D12::EnumerateAdapters()
{
    IDXGIAdapter1* adapter = nullptr;

    for (size_t counter = 0; hr == S_OK; ++counter)
    {
        adapter = nullptr;
        HRESULT hr = m_dxgiFactory->EnumAdapters1((UINT)counter, &adapter);
        MSG_TRACE_CHANNEL("RENDER SYSTEM ADAPTER INFO:", "%d", counter);
        if (adapter != nullptr)
        {
            DXGI_ADAPTER_DESC adapterDesc;
            adapter->GetDesc(&adapterDesc);
            MSG_TRACE_CHANNEL("RENDER SYSTEM ADAPTER INFO:", "vendor id: %x", adapterDesc.VendorId);
            MSG_TRACE_CHANNEL("RENDER SYSTEM ADAPTER INFO:", "device id: %x", adapterDesc.DeviceId);
            MSG_TRACE_CHANNEL("RENDER SYSTEM ADAPTER INFO:", "subsytem id: %x", adapterDesc.SubSysId);
            MSG_TRACE_CHANNEL("RENDER SYSTEM ADAPTER INFO:", "revision: %d", adapterDesc.Revision);
            MSG_TRACE_CHANNEL("RENDER SYSTEM ADAPTER INFO:", "Dedicated VRAM: %llu MiB", adapterDesc.DedicatedVideoMemory / (1024 * 1024));
            MSG_TRACE_CHANNEL("RENDER SYSTEM ADAPTER INFO:", "Dedicated RAM: %llu MiB", adapterDesc.DedicatedSystemMemory / (1024 * 1024));
            MSG_TRACE_CHANNEL("RENDER SYSTEM ADAPTER INFO:", "Shared RAM: %llu MiB", adapterDesc.SharedSystemMemory / (1024 * 1024));
            std::string str;
            convertToCString(adapterDesc.Description, str);
            MSG_TRACE_CHANNEL("RENDER SYSTEM ADAPTER INFO:", "description: %s", str.c_str());

            if (adapterDesc.VendorId != 0x8086) //THis is a hack to avoid igpus from intel, we need to do somethign else for AMD chips
            {
                break;
            }
        }
    }

    return adapter;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool DeviceManagerD3D12::createSwapChain(HWND windowHandle, int windowWidth, int windowHeight)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    HRESULT hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to creae a command queue with error: 0x%x, %s", hr, getLastErrorMessage(hr));
        return false;
    }

    // Describe and create the swap chain.
    m_swapChainDescriptor = {};
    m_swapChainDescriptor.BufferCount = m_frameCount;
    m_swapChainDescriptor.BufferDesc.Width = windowWidth;
    m_swapChainDescriptor.BufferDesc.Height = windowHeight;
    m_swapChainDescriptor.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    m_swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    m_swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    m_swapChainDescriptor.OutputWindow = windowHandle;
    m_swapChainDescriptor.SampleDesc.Count = 1;
    m_swapChainDescriptor.Windowed = TRUE;

    m_swapChainDescriptor.

        
    // Swap chain needs the queue so that it can force a flush on it.
    hr = factory->CreateSwapChain(m_commandQueue.Get(), &m_swapChainDescriptor, &m_swapChain);
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create a swap chain with error: 0x%x, %s", hr, getLastErrorMessage(hr));
        return false;
    }

    return true;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool DeviceManagerD3D12::InitialiseDebugLayers()
{
#if defined(_DEBUG)
    // Enable the D3D12 debug layer.
    HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugInterface));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create the debug interface with error: 0x%x, %s", hr, getLastErrorMessage(hr));
        return false;
    }

    m_debugInterface->EnableDebugLayer();
#endif

    return true;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool DeviceManagerD3D12::InitialiseDXGIFactory()
{

    HRESULT hr  = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("DeviceManagerD3D12", "Failed to create the DXGI interface with error: 0x%x, %s", hr, getLastErrorMessage(hr));
        return false;
    }
}
