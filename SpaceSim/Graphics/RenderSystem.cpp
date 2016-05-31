#include "Graphics/RenderSystem.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/Effect.h"
#include "Core/Settings/Settings.h"
#include "Core/Settings/SettingsManager.h"
#include <assert.h>

#include "Application/BaseApplication.h"
#include "Graphics/RenderInstance.h"
#include "Core/StringOperations/StringHelperFunctions.h"

HASH_ELEMENT_IMPLEMENTATION(CubeRendererInitialiseData);

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void CubeRendererInitialiseData::deserialise(const tinyxml2::XMLElement* element)
{
    const tinyxml2::XMLAttribute* attribute = element->FindAttribute("name");
    if (attribute)
    {
        m_texutureResourceName = attribute->Value();
    }
    attribute = element->FindAttribute("dynamic");
    if (attribute)
    {
        m_dynamic = attribute->BoolValue();
    }

    m_position.deserialise(element->FirstChildElement());
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
RenderSystem::RenderSystem() :
m_lightConstantBuffer(nullptr),
m_numberOfInstancesRenderingThisFrame(0),
m_totalNumberOfInstancesRendered(0),
m_averageNumberOfInstancesRenderingPerFrame(0),
m_totalNumberOfRenderedFrames(0)
{
    ZeroMemory(&m_swapChainDescriptor, sizeof(DXGI_SWAP_CHAIN_DESC));
    m_wireFrame = false;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
RenderSystem::~RenderSystem()
{
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "Total number of rendered instances: %d", m_totalNumberOfInstancesRendered)
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "Total number of frames: %d", m_totalNumberOfRenderedFrames)
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "Average Number of instances per frame: %d", m_averageNumberOfInstancesRenderingPerFrame)
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "Number of instances rendered last frame: %d", m_numberOfInstancesRenderingThisFrame)

    if (m_lightConstantBuffer != nullptr)
    {
        m_lightConstantBuffer->Release();
        m_lightConstantBuffer = nullptr;
    }

    if (m_cubeMapRenderer != nullptr)
    {
        delete m_cubeMapRenderer;
        m_cubeMapRenderer = nullptr;
    }

#ifdef _DEBUG
    if (m_debugAxis != nullptr)
    {
        delete m_debugAxis;
        m_debugAxis = nullptr;
    }
#endif
}


//-----------------------------------------------------------------------------
//! @brief   Initialise the RenderSystem
//! @remark  Creates a window and reveals the window
//-----------------------------------------------------------------------------
void RenderSystem::initialise(Resource* resource)
{
    m_appName = "Demo app";
    m_windowName = "Demo app";

    if (m_window.createWindow(m_appName, m_windowName))
    {
        m_window.initialise();
        m_window.showWindow();
    }

    IDXGIAdapter* adapter = nullptr;
    IDXGIFactory * pFactory;
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&pFactory));
    if (hr == S_OK)
    {
        for (size_t counter = 0; hr == S_OK; ++counter)
        {
            adapter = nullptr;
            hr = pFactory->EnumAdapters((UINT)counter, &adapter);
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

        pFactory->Release();
    }

    if (!m_deviceManager.createDevice(adapter))
    {
        ExitProcess(1); //Fix this exit cleanly
    }

    ID3D11Device* device = m_deviceManager.getDevice();
    //Patch up the factory
    patchUpDXGIFactory(device);

    int windowWidth = 1280;
    int windowHeight = 720;
    GameResourceHelper resourceHelper(resource);
    const ISetting<int>* widthSetting = resourceHelper.getGameResource().getSettingsManager().getSetting<int>("WindowWidth");
    if (widthSetting)
    {
        windowWidth = widthSetting->getData();
    }
    const ISetting<int>* heightSetting = resourceHelper.getGameResource().getSettingsManager().getSetting<int>("WindowHeight");
    if (heightSetting)
    {
        windowHeight = heightSetting->getData();
    }

    RECT rect;
    GetClientRect(m_window.getWindowHandle(), &rect);
    windowWidth = rect.right - rect.left;
    windowHeight = rect.bottom - rect.top;

    createSwapChain(device, windowWidth, windowHeight);

    setupSwapChainForRendering(device, m_deviceManager.getDeviceContext(), windowWidth, windowHeight);

    //Everything is in GL format so turn off culling
    D3D11_RASTERIZER_DESC rasterizerStateDesc;
    rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
    rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerStateDesc.AntialiasedLineEnable = false;
    rasterizerStateDesc.DepthBias = 0;
    rasterizerStateDesc.DepthBiasClamp = 0.0f;
    rasterizerStateDesc.FrontCounterClockwise = true;
    rasterizerStateDesc.MultisampleEnable = false;
    rasterizerStateDesc.ScissorEnable = false;
    rasterizerStateDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerStateDesc.DepthClipEnable = false;
    hr = device->CreateRasterizerState(&rasterizerStateDesc, &m_rasteriserState);

    D3D11_RASTERIZER_DESC rasterizerWireStateDesc;
    rasterizerWireStateDesc.CullMode = D3D11_CULL_NONE;
    rasterizerWireStateDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerWireStateDesc.AntialiasedLineEnable = false;
    rasterizerWireStateDesc.DepthBias = 0;
    rasterizerWireStateDesc.DepthBiasClamp = 0.0f;
    rasterizerWireStateDesc.FrontCounterClockwise = true;
    rasterizerWireStateDesc.MultisampleEnable = false;
    rasterizerWireStateDesc.ScissorEnable = false;
    rasterizerWireStateDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerWireStateDesc.DepthClipEnable = false;
    hr = device->CreateRasterizerState(&rasterizerWireStateDesc, &m_rasteriserWireFrameModeState);

    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    depthStencilStateDesc.DepthEnable = true;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilStateDesc.StencilEnable = false;
    depthStencilStateDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    depthStencilStateDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    depthStencilStateDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilStateDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilStateDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    hr = device->CreateDepthStencilState(&depthStencilStateDesc, &m_depthStencilState);

    D3D11_BLEND_DESC blendStateDesc;
    blendStateDesc.AlphaToCoverageEnable = false;
    blendStateDesc.IndependentBlendEnable = false;
    for (unsigned int counter = 0; counter < 8; ++counter)
    {
        blendStateDesc.RenderTarget[counter].BlendEnable = false;
        blendStateDesc.RenderTarget[counter].SrcBlend = D3D11_BLEND_ONE;
        blendStateDesc.RenderTarget[counter].DestBlend = D3D11_BLEND_ZERO;
        blendStateDesc.RenderTarget[counter].BlendOp = D3D11_BLEND_OP_ADD;
        blendStateDesc.RenderTarget[counter].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendStateDesc.RenderTarget[counter].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendStateDesc.RenderTarget[counter].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendStateDesc.RenderTarget[counter].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }
    hr = device->CreateBlendState(&blendStateDesc, &m_blendState);

    D3D11_BLEND_DESC blendDescriptor;
    blendDescriptor.AlphaToCoverageEnable = FALSE;
    blendDescriptor.IndependentBlendEnable = FALSE;
    for (unsigned int counter = 0; counter < 8; ++counter)
    {
        blendDescriptor.RenderTarget[0].BlendEnable = TRUE;
        blendDescriptor.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDescriptor.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDescriptor.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDescriptor.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDescriptor.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDescriptor.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDescriptor.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    }

    hr = device->CreateBlendState(&blendDescriptor, &m_alphaBlendState);

    m_textureManager.createSamplerStates(m_deviceManager);

    D3D11_BUFFER_DESC lightContantsDescriptor;
    ZeroMemory(&lightContantsDescriptor, sizeof(D3D11_BUFFER_DESC));
    lightContantsDescriptor.ByteWidth = sizeof(LightConstants) * 8 + 4 * sizeof(float);
    lightContantsDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&lightContantsDescriptor, 0, &m_lightConstantBuffer);

#ifdef _DEBUG
    m_debugAxis = new OrientationAxis();
    m_debugAxis->initialise(resource, m_deviceManager);


    hr = m_deviceManager.getDeviceContext()->QueryInterface(__uuidof(pPerf), reinterpret_cast<void**>(&pPerf));
#endif

    initialiseCubemapRendererAndResources(resourceHelper);

}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void RenderSystem::update(Resource* resource, RenderInstanceTree& renderInstances, float elapsedTime, double time)
{
    static const unsigned int defaultTechniqueHash = hashString("default");
    m_window.update(elapsedTime, time);

    ID3D11DeviceContext* deviceContext = m_deviceManager.getDeviceContext();

    RenderInstanceTree::iterator renderInstanceIt = renderInstances.begin();
    RenderInstanceTree::iterator renderInstanceEnd = renderInstances.end();
    unsigned int stride = 0;
    unsigned int offset = 0;

    GameResourceHelper gameResource = GameResourceHelper(resource);
    const ShaderCache& shaderCache = gameResource.getGameResource().getShaderCache();

    size_t oldTechniqueId = 0;
    for (; renderInstanceIt != renderInstanceEnd; ++renderInstanceIt)
    {
        RenderInstance& renderInstance = (RenderInstance&)(*(*renderInstanceIt));
#ifdef _DEBUG
        if (!renderInstance.m_name.empty())
        {
            pPerf->BeginEvent(renderInstance.m_name.c_str());
        }
#endif
        const Material& material = renderInstance.getShaderInstance().getMaterial();
        const Effect* effect = material.getEffect();
        const Technique* technique = effect->getTechnique(material.getTechnique());
        technique->setMaterialContent(m_deviceManager, material.getMaterialCB());
        technique->setWVPContent(m_deviceManager, renderInstance.getShaderInstance().getWVPConstants());

        const std::vector<unsigned int>& textureHashes = material.getTextureHashes();
        //const std::vector<ID3D11SamplerState*>& samplerStates = renderInstance.getMaterial().getTextureSamplers();
        for (unsigned int counter = 0; counter < textureHashes.size(); ++counter)
        {
            const Texture* texture = m_textureManager.getTexture(textureHashes[counter]);
            ID3D11ShaderResourceView* srv = texture->getShaderResourceView();
            deviceContext->PSSetShaderResources(counter, 1, &srv);
            ID3D11SamplerState* const samplerState = m_textureManager.getSamplerState();
            deviceContext->PSSetSamplers(counter, 1, &samplerState);
        }

        if (technique->getTechniqueId() != oldTechniqueId)
        {
            //this will crash, also we shouldnt set this if the shader id hasnt changed from the previous set
            deviceContext->VSSetShader(shaderCache.getVertexShader(technique->getVertexShader()) ? shaderCache.getVertexShader(technique->getVertexShader())->getShader() : nullptr, nullptr, 0);
            deviceContext->HSSetShader(shaderCache.getHullShader(technique->getHullShader()) ? shaderCache.getHullShader(technique->getHullShader())->getShader() : nullptr, nullptr, 0);
            deviceContext->DSSetShader(shaderCache.getDomainShader(technique->getDomainShader()) ? shaderCache.getDomainShader(technique->getDomainShader())->getShader() : nullptr, nullptr, 0);
            deviceContext->GSSetShader(shaderCache.getGeometryShader(technique->getGeometryShader()) ? shaderCache.getGeometryShader(technique->getGeometryShader())->getShader() : nullptr, nullptr, 0);
            deviceContext->PSSetShader(shaderCache.getPixelShader(technique->getPixelShader()) ? shaderCache.getPixelShader(technique->getPixelShader())->getShader() : nullptr, nullptr, 0);
            oldTechniqueId = technique->getTechniqueId();
        }
        technique->setupTechnique();

        deviceContext->PSSetConstantBuffers(1, 1, &m_lightConstantBuffer);
        if (material.getBlendState())
        {
            deviceContext->OMSetBlendState(m_alphaBlendState, 0, 0xffffffff);
        }
        else
        {
            deviceContext->OMSetBlendState(m_blendState, 0, 0xffffffff);
        }

        // Set vertex buffer stride and offset.
        const VertexBuffer* vb = renderInstance.getGeometryInstance().getVB();
        ID3D11Buffer* buffer = vb->getBuffer();
        stride = static_cast<unsigned int>(vb->getVertexStride()); 
        if (renderInstance.getGeometryInstance().getIB() != nullptr)
        {
            deviceContext->IASetInputLayout(renderInstance.getGeometryInstance().getVB()->getInputLayout());
            deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
            deviceContext->IASetIndexBuffer(renderInstance.getGeometryInstance().getIB()->getBuffer(), DXGI_FORMAT_R32_UINT, 0);
            deviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)renderInstance.getPrimitiveType());
            deviceContext->DrawIndexed(renderInstance.getGeometryInstance().getIB()->getNumberOfIndecis(), 0, 0);
        }
        else
        {
            deviceContext->IASetInputLayout(renderInstance.getGeometryInstance().getVB()->getInputLayout());
            deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
            deviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)renderInstance.getPrimitiveType());
            deviceContext->Draw((unsigned int)renderInstance.getGeometryInstance().getVB()->getVertexCount(), 0);
        }
#ifdef _DEBUG
        if (!renderInstance.m_name.empty())
        {
            pPerf->EndEvent();
        }
#endif
    }

    m_numberOfInstancesRenderingThisFrame = renderInstances.size();
    m_totalNumberOfInstancesRendered += m_numberOfInstancesRenderingThisFrame;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool RenderSystem::createSwapChain(ID3D11Device* device, int windowWidth, int windowHeight)
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
    m_swapChainDescriptor.OutputWindow = m_window.getWindowHandle();
    m_swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    m_swapChainDescriptor.SampleDesc.Count = 1;
    m_swapChainDescriptor.SampleDesc.Quality = 0;
    m_swapChainDescriptor.Windowed = true;

    if (FAILED(m_dxgiFactory->CreateSwapChain((IUnknown*)device, &m_swapChainDescriptor, &m_swapChain)))
    {
        MSG_TRACE_CHANNEL("ERROR", "Failed to create a swapchain")
            return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
//! @brief   Because the device is created elsewhere(DeviceManager) the DXGIFactory
//!          needs to be patched. Otherwise other resources can't be created through it.
//! @remark
//-----------------------------------------------------------------------------
void RenderSystem::patchUpDXGIFactory(ID3D11Device* device)
{
    IDXGIDevice * pDXGIDevice;
    IUnknown* deviceinterface = (IUnknown*)device;
    HRESULT hr = deviceinterface->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
    IDXGIAdapter * pDXGIAdapter;
    hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("ERROR", "Failed to path the DXGI Factory");
        return;
    }
    pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_dxgiFactory);
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void RenderSystem::setupSwapChainForRendering(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int windowWidth, int windowHeight)
{
    // Create a render target view
    if (FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_backBuffer)))
    {
        MSG_TRACE_CHANNEL("ERROR", "Failed to acquire the back buffer pointer")
            return;
    }

    if (FAILED(device->CreateRenderTargetView(m_backBuffer, NULL, &m_renderTargetView)))
    {
        MSG_TRACE_CHANNEL("ERROR", "Failed to Create the render target view")
            return;
    }
    m_backBuffer->Release();

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC depthBufferDescriptor;
    ZeroMemory(&depthBufferDescriptor, sizeof(D3D11_TEXTURE2D_DESC));
    depthBufferDescriptor.Width = windowWidth;
    depthBufferDescriptor.Height = windowHeight;
    depthBufferDescriptor.MipLevels = 1;
    depthBufferDescriptor.ArraySize = 1;
    depthBufferDescriptor.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDescriptor.SampleDesc.Count = 1;
    depthBufferDescriptor.SampleDesc.Quality = 0;
    depthBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDescriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDescriptor.CPUAccessFlags = 0;
    depthBufferDescriptor.MiscFlags = 0;
    if (FAILED(device->CreateTexture2D(&depthBufferDescriptor, NULL, &m_depthStencilBuffer)))
    {
        MSG_TRACE_CHANNEL("ERROR", "Failed to create the back buffer")
            return;
    }

#if _DEBUG //Name the depth Stencil buffer
    //m_depthStencilBuffer->SetPrivateData( WKPDID_D3DDebugObjectName, 18, "DepthStencilBuffer" );
#endif

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescriptor;
    ZeroMemory(&depthStencilViewDescriptor, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    depthStencilViewDescriptor.Format = depthBufferDescriptor.Format;
    depthStencilViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDescriptor.Texture2D.MipSlice = 0;
    if (FAILED(device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDescriptor, &m_depthStencilView)))
    {
        MSG_TRACE_CHANNEL("ERROR", "Failed to create the depth stencil view")
            return;
    }

    deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

#if _DEBUG //Name the target views
    //m_depthStencilView->SetPrivateData( WKPDID_D3DDebugObjectName, 16, "DepthStencilView" );
    //m_renderTargetView->SetPrivateData( WKPDID_D3DDebugObjectName, 16, "RenderTargetView" );
#endif

    //// Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = (float)windowWidth;
    vp.Height = (float)windowHeight;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    deviceContext->RSSetViewports(1, &vp);
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void RenderSystem::cleanup()
{
    if (m_dxgiFactory)
    {
        m_dxgiFactory->Release();
    }
    if (m_swapChain)
    {
        m_swapChain->Release();
    }
    if (m_depthStencilBuffer)
    {
        m_depthStencilBuffer->Release();
    }
    if (m_renderTargetView)
    {
        m_renderTargetView->Release();
    }
    if (m_depthStencilView)
    {
        m_depthStencilView->Release();
    }
}

//-------------------------------------------------------------------------
// @brief Takes a renderlist to do ordering on the render list before actually rendering it
//-------------------------------------------------------------------------
void RenderSystem::beginDraw(RenderInstanceTree& renderInstances, Resource* resource)
{
    float clearColor[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
    ID3D11DeviceContext* deviceContext = m_deviceManager.getDeviceContext();
    deviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);
    deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    if (!m_wireFrame)
    {
        deviceContext->RSSetState(m_rasteriserState);
    }
    else
    {
        deviceContext->RSSetState(m_rasteriserWireFrameModeState);
    }
    deviceContext->OMSetBlendState(m_blendState, 0, 0xffffffff);
    deviceContext->OMSetDepthStencilState(m_depthStencilState, 0xffffffff);

    std::sort(begin(renderInstances), end(renderInstances), [=](const RenderInstance* lhs, const RenderInstance* rhs)
    {
        const Material& lhsMaterial = lhs->getShaderInstance().getMaterial();
        const Material& rhsMaterial = rhs->getShaderInstance().getMaterial();
        //This might not be the most effiecient material/sahder id combo
        return lhsMaterial.getEffect()->getTechnique(lhsMaterial.getTechnique())->getTechniqueId() < rhsMaterial.getEffect()->getTechnique(rhsMaterial.getTechnique())->getTechniqueId();
    });

    std::sort(begin(renderInstances), end(renderInstances), [=](const RenderInstance* lhs, const RenderInstance* rhs)
    {
        return lhs->getShaderInstance().getMaterial().getBlendState() < rhs->getShaderInstance().getMaterial().getBlendState();
    });

    //Setup light constants they might change during the frame, only support up to 8 lights for now as we do forward shading
    GameResourceHelper helper(resource);
    const CameraManager& cm = helper.getGameResource().getCameraManager();
    LightManager& lm = helper.getWritableGameResource().getLightManager();

    const Camera* cam = cm.getCamera("global");
    Vector3 camPos;
    if (cam != nullptr)
    {
        camPos = cam->getEye();
    }
    PerFrameConstants perFrameConstants;
    ZeroMemory(&perFrameConstants.m_lightConstants, sizeof(LightConstants) * 8);
    Light* light0 = lm.getLight("light_0");
    if (light0 != nullptr)
    {
        light0->setPosition(camPos);
    }

    unsigned int lightCounter = 0;
    for (auto light : lm.getLights())
    {
        perFrameConstants.m_lightConstants[lightCounter] = light->getLightConstants();
        ++lightCounter;
    }

    static int counter = 0;
    if (counter > 15)
    {
        //Loop over rts here and render to all of them
        TextureManager& tm = helper.getWritableGameResource().getTextureManager();
        static size_t rtCounter = 0;
        rtCounter = (++rtCounter) % m_cubeSettings.size();
        if (m_cubeSettings[rtCounter].m_dynamic || m_cubeSettings[rtCounter].m_hasBeenRenderedTo == false)
        {
            ID3D11ShaderResourceView* srv[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
            deviceContext->PSSetShaderResources(0, 8, srv);
            m_cubeMapRenderer->initialise(m_cubeSettings[rtCounter].m_position);
            m_cubeMapRenderer->renderCubeMap(resource, const_cast<Texture*>( tm.getTexture(m_cubeSettings[rtCounter].m_texutureResourceName) ), renderInstances, m_deviceManager, perFrameConstants, tm);
            m_cubeSettings[rtCounter].m_hasBeenRenderedTo = true;
            deviceContext->PSSetShaderResources(0, 8, srv);
        }
        counter = 0;
    }
    ++counter;

    perFrameConstants.m_cameraPosition[0] = camPos.x();
    perFrameConstants.m_cameraPosition[1] = camPos.y();
    perFrameConstants.m_cameraPosition[2] = camPos.z();
    deviceContext->UpdateSubresource(m_lightConstantBuffer, 0, 0, (void*)&perFrameConstants, 0, 0);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void RenderSystem::endDraw(Resource* resource)
{
#ifdef _DEBUG
    m_debugAxis->draw(m_deviceManager, resource);
#else
    UNUSEDPARAM(resource);
#endif

    HRESULT hr = m_swapChain->Present(0, 0);
    ++m_totalNumberOfRenderedFrames;
    m_averageNumberOfInstancesRenderingPerFrame = m_totalNumberOfInstancesRendered / m_totalNumberOfRenderedFrames;
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("ERROR", "Present call failed with error code: 0x%x", hr)
            MSG_TRACE_CHANNEL("ERROR", "Device removed because : 0x%x", m_deviceManager.getDevice()->GetDeviceRemovedReason())
            assert(false);
    }
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void RenderSystem::initialiseCubemapRendererAndResources(GameResourceHelper &resourceHelper)
{
    m_cubeMapRenderer = new CubeMapRenderer(m_deviceManager, m_alphaBlendState, m_blendState);

    const Paths& path = resourceHelper.getGameResource().getPaths();
    std::string cubeRenderSettingsFileName = path.getSettingsPath() + "cube_map_renderers.xml";
    tinyxml2::XMLDocument doc;
    if (tinyxml2::XML_NO_ERROR != doc.LoadFile(cubeRenderSettingsFileName.c_str()))
    {
        MSG_TRACE_CHANNEL("RENDERSYSTEM ERROR", "Failed to load %s because %s (%s)", cubeRenderSettingsFileName.c_str(), doc.GetErrorStr1(), doc.GetErrorStr2());
    }

    tinyxml2::XMLElement* element = doc.FirstChildElement();
    if (element == nullptr)
    {
        MSG_TRACE_CHANNEL("RENDERSYSTEM ERROR", "First Element is nullptr");
        return;
    }
    element = element->FirstChildElement();
    for (element; element; element = element->NextSiblingElement())
    {
        unsigned int elementHash = hashString(element->Value());
        if (elementHash == CubeRendererInitialiseData::m_hash)
        {
            CubeRendererInitialiseData currentCubeSetting;
            currentCubeSetting.deserialise(element);
            currentCubeSetting.m_hasBeenRenderedTo = false;
            m_cubeSettings.push_back(currentCubeSetting);
        }
    }

    std::sort(begin(m_cubeSettings), end(m_cubeSettings), [=](const CubeRendererInitialiseData& lhs, const CubeRendererInitialiseData& rhs) { return lhs.m_dynamic < rhs.m_dynamic; });

    unsigned int cubeMapWidhtHeight = 1024;

    D3D11_TEXTURE2D_DESC cubeDesc;
    ZeroMemory(&cubeDesc, sizeof(D3D11_TEXTURE3D_DESC));
    cubeDesc.Width = cubeMapWidhtHeight;
    cubeDesc.Height = cubeMapWidhtHeight;
    cubeDesc.MipLevels = 0;
    cubeDesc.ArraySize = 6;
    cubeDesc.SampleDesc.Count = 1;
    cubeDesc.SampleDesc.Quality = 0;
    cubeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    cubeDesc.Usage = D3D11_USAGE_DEFAULT;
    cubeDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    cubeDesc.CPUAccessFlags = 0;
    cubeDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;

    //Should create a 6 2D rts here instead
    D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
    rtDesc.Format = cubeDesc.Format;
    rtDesc.Texture2DArray.FirstArraySlice = 0;
    rtDesc.Texture2DArray.ArraySize = 6;
    rtDesc.Texture2DArray.MipSlice = 0;
    rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;

    // Create the shader resource view for the cubic env map
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = cubeDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    srvDesc.TextureCube.MipLevels = 1;
    srvDesc.TextureCube.MostDetailedMip = 0;

    TextureManager& tm = resourceHelper.getWritableGameResource().getTextureManager();

    for (size_t counter = 0; counter < m_cubeSettings.size(); ++counter)
    {
        Texture cubeMap;
        HRESULT hr = S_FALSE;

        ID3D11Texture2D* textureResource;
        ID3D11RenderTargetView* rtView[6];
        ID3D11ShaderResourceView* srView;
        hr = m_deviceManager.getDevice()->CreateTexture2D(&cubeDesc, nullptr, &textureResource);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("CubemapRenderer_ERROR", "Failed to create texture resource for the cubemap renderer: 0x%x", hr);
            return;
        }

        rtDesc.Texture2DArray.ArraySize = 1;
        for (size_t rtCounter = 0; rtCounter < 6; ++rtCounter)
        {
            rtDesc.Texture2DArray.FirstArraySlice = (unsigned int)rtCounter;
            hr = m_deviceManager.getDevice()->CreateRenderTargetView(textureResource, &rtDesc, &rtView[rtCounter]);

            if (FAILED(hr))
            {
                MSG_TRACE_CHANNEL("CubemapRenderer_ERROR", "Failed to create render target view for the cubemap renderer: 0x%x", hr);
                return;
            }
        }

        hr = m_deviceManager.getDevice()->CreateShaderResourceView(textureResource, &srvDesc, &srView);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("CubemapRenderer_ERROR", "Failed to create shader resource view for the cubemap renderer: 0x%x", hr);
            return;
        }

        cubeMap.createRenderTarget(textureResource, rtView, srView);
        tm.addTexture(m_cubeSettings[counter].m_texutureResourceName, cubeMap);
    }
}
