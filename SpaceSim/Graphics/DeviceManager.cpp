#include "Graphics/DeviceManager.h"
#include "Graphics/D3DDebugHelperFunctions.h"
#include <iostream>

#include "Core/StringOperations/StringHelperFunctions.h"

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
DeviceManagerD3D11::DeviceManagerD3D11()
{
    ZeroMemory(&m_swapChainDescriptor, sizeof(DXGI_SWAP_CHAIN_DESC));
}


///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
bool DeviceManagerD3D11::createDevice()
{
    IDXGIAdapter* adapter = nullptr;
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&m_dxgiFactory));
    if (hr == S_OK)
    {
        for (size_t counter = 0; hr == S_OK; ++counter)
        {
            adapter = nullptr;
            hr = m_dxgiFactory->EnumAdapters((UINT)counter, &adapter);
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

                if (adapterDesc.VendorId != 0x8086)
                {
                    break;
                }
            }
        }

        if (adapter != nullptr)
        {
            MSG_TRACE_CHANNEL("DEVICEMANAGER", "Creating Device");
            //Figure out what the highest feature level is we support
            D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_9_1;
            hr = D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, 0, 0, 0, D3D11_SDK_VERSION, 0, &featureLevel, 0);
            if (FAILED(hr))
            {
                MSG_TRACE_CHANNEL("DEVICEMANAGER", "Failed to create a D3D device with error code: 0x%x", hr)
                    return false;
            }

            MSG_TRACE_CHANNEL("DEVICEMANAGER", "Selected Feature level: 0x%x", featureLevel)

                DWORD deviceCreationFlags = 0/*D3D11_CREATE_DEVICE_SINGLETHREADED*/;
#ifdef _DEBUG
            deviceCreationFlags |= D3D11_CREATE_DEVICE_DEBUG;//| D3D11_CREATE_DEVICE_DEBUGGABLE;
#endif
            hr = D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, deviceCreationFlags, &featureLevel, 1, D3D11_SDK_VERSION, &m_device, &m_featureLevel, &m_deviceContext);
            if (FAILED(hr))
            {
                MSG_TRACE_CHANNEL("DEVICEMANAGER", "Failed to create a D3D device with error code: 0x%x", hr)
                return false;
            }

            hr = m_device->CreateDeferredContext(0, &m_deferredContext);
            if (FAILED(hr))
            {
                MSG_TRACE_CHANNEL("DEVICEMANAGER", "Failed to create a deferred device context with error: 0x%x", hr);
                return false;
            }

            hr = m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM, 4, &m_qualityLevels);
            if (FAILED(hr))
            {
                MSG_TRACE_CHANNEL("DEVICEMANAGER", "Failed to Get the quality levels for the back buffer multisampling with error: 0x%x", hr);
                return false;
            }

#ifdef _DEBUG
            m_device->SetPrivateData(WKPDID_D3DDebugObjectName, 13, "Render Device");
            m_debugInterface = nullptr;
            m_device->QueryInterface(__uuidof(ID3D11Debug), (void **)&m_debugInterface);
#endif

            D3DDebugHelperFunctions::SetDebugChildName(m_deviceContext, "Render Device Context");

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
bool DeviceManagerD3D11::createSwapChain(HWND windowHandle, int windowWidth, int windowHeight)
{

    DXGI_MODE_DESC bufferDescription;
    ZeroMemory(&bufferDescription, sizeof(DXGI_MODE_DESC));
    bufferDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    bufferDescription.Height = windowHeight;
    bufferDescription.Width = windowWidth;
    bufferDescription.RefreshRate.Numerator = 60;
    bufferDescription.RefreshRate.Denominator = 1;

    m_swapChainDescriptor.BufferCount = 1;
    m_swapChainDescriptor.BufferDesc = bufferDescription;
    m_swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    m_swapChainDescriptor.OutputWindow = windowHandle;
    m_swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    m_swapChainDescriptor.SampleDesc.Count = 4;
    m_swapChainDescriptor.SampleDesc.Quality = 0;
    m_swapChainDescriptor.Windowed = true;

    if (FAILED(m_dxgiFactory->CreateSwapChain((IUnknown*)m_device, &m_swapChainDescriptor, &m_swapChain)))
    {
        MSG_TRACE_CHANNEL("ERROR", "Failed to create a swapchain")
            return false;
    }

    return true;
}

///-----------------------------------------------------------------------------
///! @brief  Cleanup the device, D3D object and swap chains
///! @remark
///-----------------------------------------------------------------------------
void DeviceManagerD3D11::cleanup()
{
    if (m_deviceContext)
    {
        m_deviceContext->Release();
    }

    if (m_swapChain)
    {
        m_swapChain->Release();
    }
    
    if (m_device)
    {
        m_device->Release();
    }

    if (m_dxgiFactory)
    {
        m_dxgiFactory->Release();
    }

#ifdef _DEBUG
    if (m_debugInterface != nullptr)
    {
        m_debugInterface->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
        m_debugInterface->Release();
    }
#endif
}