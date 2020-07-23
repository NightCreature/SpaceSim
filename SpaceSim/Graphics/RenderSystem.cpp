#include "Graphics/RenderSystem.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/Effect.h"
#include "Core/Settings/Settings.h"
#include "Core/Settings/SettingsManager.h"
#include <assert.h>

#include "Application/BaseApplication.h"
#include "Graphics/RenderInstance.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/MessageSystem/MessageQueue.h"

#include "Graphics/ShaderPack.h"
#include "Graphics/Frustum.h"

#include "Core/Profiler/ProfilerMacros.h"

#include "Core/MessageSystem/GameMessages.h"
#include "Graphics/D3DDebugHelperFunctions.h"



///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
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

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
RenderSystem::RenderSystem() :
m_lightConstantBuffer(nullptr),
m_shadowConstantBuffer(nullptr),
m_numberOfInstancesRenderingThisFrame(0),
m_totalNumberOfInstancesRendered(0),
m_averageNumberOfInstancesRenderingPerFrame(0),
m_totalNumberOfRenderedFrames(0)
{
    m_wireFrame = false;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
RenderSystem::~RenderSystem()
{
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "Total number of rendered instances: %d", m_totalNumberOfInstancesRendered)
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "Total number of frames: %d", m_totalNumberOfRenderedFrames)
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "Average Number of instances per frame: %d", m_averageNumberOfInstancesRenderingPerFrame)
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "Number of instances rendered last frame: %d", m_numberOfInstancesRenderingThisFrame)

    m_shadowMapRenderer->cleanup();
    m_cubeMapRenderer->cleanup();

    if (m_lightConstantBuffer != nullptr)
    {
        m_lightConstantBuffer->Release();
        m_lightConstantBuffer = nullptr;
    }

    if (m_shadowConstantBuffer != nullptr)
    {
        m_shadowConstantBuffer->Release();
        m_shadowConstantBuffer = nullptr;
    }

    if (m_cubeMapRenderer != nullptr)
    {
        delete m_cubeMapRenderer;
        m_cubeMapRenderer = nullptr;
    }

    if (m_shadowMapRenderer)
    {
        delete m_shadowMapRenderer;
        m_shadowMapRenderer = nullptr;
    }

    m_textureManager.cleanup();
    m_modelManger.cleanup(); //just to see if it matters in the live objects list
    m_shaderCache.cleanup();
    m_effectCache.cleanup();
   

#ifdef _DEBUG
    pPerf->Release();
    m_debugAxis->cleanup();
#endif

#ifdef _DEBUG
    if (m_debugAxis != nullptr)
    {
        delete m_debugAxis;
        m_debugAxis = nullptr;
    }
#endif
}


///-----------------------------------------------------------------------------
///! @brief   Initialise the RenderSystem
///! @remark  Creates a window and reveals the window
///-----------------------------------------------------------------------------
void RenderSystem::initialise(Resource* resource)
{
    m_renderResource = new RenderResource(resource->m_logger, resource->m_messageQueues, resource->m_paths, resource->m_performanceTimer, resource->m_settingsManager, &m_cameraSystem, &m_deviceManager, &m_effectCache, &m_window, &m_lightManager, &m_modelManger, &m_resourceLoader, &m_shaderCache, &m_textureManager);

    m_modelManger.initialise(m_renderResource);
    m_resourceLoader.initialise(m_renderResource);

    m_appName = "Demo app";
    m_windowName = "Demo app";

    if (m_window.createWindow(m_appName, m_windowName))
    {
        m_window.initialise();
        m_window.showWindow();
    }

    if (!m_deviceManager.createDevice())
    {
        ExitProcess(1); //Fix this exit cleanly
    }

    int windowWidth = 1280;
    int windowHeight = 720;
    const ISetting<int>* widthSetting = resource->m_settingsManager->getSetting<int>("WindowWidth");
    if (widthSetting)
    {
        windowWidth = widthSetting->getData();
    }
    const ISetting<int>* heightSetting = resource->m_settingsManager->getSetting<int>("WindowHeight");
    if (heightSetting)
    {
        windowHeight = heightSetting->getData();
    }

    m_CullingProjectionMatrix = math::createLeftHandedFOVPerspectiveMatrix(math::gmPI / 4.0f, (float)windowWidth / (float)windowHeight, 1000.0f, 0.001f); //Reverse Z

    RECT rect;
    GetClientRect(m_window.getWindowHandle(), &rect);
    windowWidth = rect.right - rect.left;
    windowHeight = rect.bottom - rect.top;

    if (m_deviceManager.createSwapChain(m_window.getWindowHandle(), windowWidth, windowHeight))
    {
        m_swapChain = m_deviceManager.GetSwapChain();
    }

    auto device = m_deviceManager.getDevice();
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
    HRESULT hr = device->CreateRasterizerState(&rasterizerStateDesc, &m_rasteriserState);
    D3DDebugHelperFunctions::SetDebugChildName(m_rasteriserState, "RenderSystem RasterizerState");

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
    D3DDebugHelperFunctions::SetDebugChildName(m_rasteriserWireFrameModeState, "RenderSystem WireFrameRasterizerState");
    
    D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
    depthStencilStateDesc.DepthEnable = true;
    depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_GREATER;
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
    D3DDebugHelperFunctions::SetDebugChildName(m_depthStencilState, "RenderSystem DepthStencilState");

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
    D3DDebugHelperFunctions::SetDebugChildName(m_blendState, "RenderSystem BlendState");

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
    D3DDebugHelperFunctions::SetDebugChildName(m_alphaBlendState, "RenderSystem AlphaBlendState");
    
    m_textureManager.createSamplerStates(m_deviceManager);

    D3D11_BUFFER_DESC lightContantsDescriptor;
    ZeroMemory(&lightContantsDescriptor, sizeof(D3D11_BUFFER_DESC));
    lightContantsDescriptor.ByteWidth = sizeof(LightConstants) * 8 + 4 * sizeof(float) + 3 * 16 * sizeof(float);
    lightContantsDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&lightContantsDescriptor, 0, &m_lightConstantBuffer);
    D3DDebugHelperFunctions::SetDebugChildName(m_lightConstantBuffer, "RenderSystem Light Constant buffer");

    D3D11_BUFFER_DESC shadowContantsDescriptor;
    ZeroMemory(&shadowContantsDescriptor, sizeof(D3D11_BUFFER_DESC));
    shadowContantsDescriptor.ByteWidth = 3 * 16 * sizeof(float);
    shadowContantsDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = device->CreateBuffer(&shadowContantsDescriptor, 0, &m_shadowConstantBuffer);
    D3DDebugHelperFunctions::SetDebugChildName(m_shadowConstantBuffer, "RenderSystem Shadow Constant buffer");

    D3D11_SAMPLER_DESC samplerStateDesc;
    samplerStateDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerStateDesc.MipLODBias = 0.0f;
    samplerStateDesc.MaxAnisotropy = 16;
    samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerStateDesc.BorderColor[0] = 0.0f;
    samplerStateDesc.BorderColor[1] = 0.0f;
    samplerStateDesc.BorderColor[2] = 0.0f;
    samplerStateDesc.BorderColor[3] = 0.0f;
    samplerStateDesc.MinLOD = -3.402823466e+38F;
    samplerStateDesc.MaxLOD = 3.402823466e+38F;
    hr = device->CreateSamplerState(&samplerStateDesc, &m_samplerState);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("RENDER SYSTEM", "Failed to create sampler state: 0x%x", hr)
    }
    D3DDebugHelperFunctions::SetDebugChildName(m_samplerState, "RenderSystem SamplerState");

#ifdef _DEBUG
    m_debugAxis = new OrientationAxis();
    m_debugAxis->initialise(m_renderResource, m_deviceManager);


    hr = m_deviceManager.getDeviceContext()->QueryInterface(__uuidof(pPerf), reinterpret_cast<void**>(&pPerf));
#endif

    ShaderPack shaderPack(m_renderResource);
    shaderPack.loadShaderPack("shader_pack.xml");

    MSG_TRACE_CHANNEL("RENDERSYSTEM", "HARDCODED CAMERAS IN USE THESE SHOULD BE CREATED THROUGH MESSAGES");
    m_cameraSystem.createCamera(*m_renderResource, "global", Vector3(0.0f, 0.0f, 200.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3::yAxis());
    m_cameraSystem.createCamera(*m_renderResource, "text_block_camera", Vector3(0.0f, 0.0f, 200.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3::yAxis());
    m_cameraSystem.createCamera(*m_renderResource, "player_camera", Vector3(0.0f, 0.0f, 200.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3::yAxis());

    initialiseCubemapRendererAndResources(m_renderResource);
    m_shadowMapRenderer = new ShadowMapRenderer(m_deviceManager, m_blendState, m_alphaBlendState);

    m_messageObservers.AddDispatchFunction(MESSAGE_ID(CreateLightMessage), fastdelegate::MakeDelegate(&m_lightManager, &LightManager::dispatchMessage));
    m_messageObservers.AddDispatchFunction(MESSAGE_ID(LoadResourceRequest), fastdelegate::MakeDelegate(&m_resourceLoader, &ResourceLoader::dispatchMessage));
    m_messageObservers.AddDispatchFunction(MESSAGE_ID(RenderInformation), fastdelegate::MakeDelegate(this, &RenderSystem::CreateRenderList));

    m_projection = math::createLeftHandedFOVPerspectiveMatrix(math::gmPI / 4.0f, (float)windowWidth / (float)windowHeight, 1500.0f, 0.001f);
    m_view = m_cameraSystem.getCamera("global")->getCamera();

    m_emmiter.initialise(m_renderResource);

    //Create some querries
    D3D11_QUERY_DESC queryDescriptor;
    ZeroMemory(&queryDescriptor, sizeof(queryDescriptor));
    queryDescriptor.Query = D3D11_QUERY_TIMESTAMP;
    device->CreateQuery(&queryDescriptor, &m_beginDrawQuery);
    device->CreateQuery(&queryDescriptor, &m_endDrawQuery);
    device->CreateQuery(&queryDescriptor, &m_cubemapBeginDrawQuery);
    device->CreateQuery(&queryDescriptor, &m_cubemapEndDrawQuery);
    device->CreateQuery(&queryDescriptor, &m_shadoBeginDrawQuery);
    device->CreateQuery(&queryDescriptor, &m_shadoEndDrawQuery);
    device->CreateQuery(&queryDescriptor, &m_mainBeginDrawQuery);
    device->CreateQuery(&queryDescriptor, &m_mainEndDrawQuery);

    ZeroMemory(&queryDescriptor, sizeof(queryDescriptor));
    queryDescriptor.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
    device->CreateQuery(&queryDescriptor, &m_timeDisjointQuery);

    ZeroMemory(&queryDescriptor, sizeof(queryDescriptor));
    queryDescriptor.Query = D3D11_QUERY_PIPELINE_STATISTICS;
    device->CreateQuery(&queryDescriptor, &m_pipeLineStatistics);
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void RenderSystem::update(float elapsedTime, double time)
{
//    {
//#ifdef _DEBUG
//        pPerf->BeginEvent(L"Emitter Update");
//#endif
//        m_emmiter.update((double)elapsedTime, m_view, m_projection, m_inverseView); //This fucks with the pipeline state :(
//#ifdef _DEBUG
//        pPerf->EndEvent();
//#endif
//    }
//
    PROFILE_EVENT("RenderSystem::Update", Blue);
    ID3D11DeviceContext* deviceContext = m_deviceManager.getDeviceContext();
    deviceContext->End(m_mainBeginDrawQuery);

#ifdef _DEBUG
    pPerf->BeginEvent(L"RenderSystem::Update");
#endif

    const auto defaultTechniqueHash = "default"_hash;
    m_window.update(elapsedTime, time);

    
    deviceContext->VSSetConstantBuffers(0, 1, &m_lightConstantBuffer);

    RenderInstanceTree::iterator renderInstanceIt = visibleInstances.begin();
    RenderInstanceTree::iterator renderInstanceEnd = visibleInstances.end();
    unsigned int stride = 0;
    unsigned int offset = 0;

    RenderResourceHelper gameResource = { m_renderResource };
    const ShaderCache& shaderCache = gameResource.getResource().getShaderCache();
    
    size_t oldTechniqueId = 0;
    std::vector<ID3D11ShaderResourceView*> resourceViews;
    std::vector<ID3D11SamplerState*> samplerStates;
    {
        PROFILE_EVENT("RenderSystem::ReserveVectors", OrangeRed);
        resourceViews.reserve(128);
        samplerStates.reserve(2);

        samplerStates.push_back(m_textureManager.getSamplerState());
        samplerStates.push_back(m_samplerState);
        deviceContext->PSSetSamplers(0, 2, &samplerStates[0]);
    }

    for (; renderInstanceIt != renderInstanceEnd; ++renderInstanceIt)
    {
        {
            PROFILE_EVENT("RenderSystem::SubmitInstance", DarkBlue);
            resourceViews.clear();

            RenderInstance& renderInstance = (RenderInstance&)(*(*renderInstanceIt));
#ifdef _DEBUG
            //if (!renderInstance.m_name.empty())
            //{
            //    pPerf->BeginEvent(renderInstance.m_name.c_str());
            //}
#endif
            const auto& shaderInstance = renderInstance.getShaderInstance();
            const Effect* effect = m_effectCache.getEffect(shaderInstance.getMaterial().getEffectHash());;
            const Technique* technique = effect->getTechnique(shaderInstance.getMaterial().getTechnique());
            
            if (!shaderInstance.getVSConstantBufferSetup().empty())
            {
                deviceContext->VSSetConstantBuffers(2, static_cast<uint32>(shaderInstance.getVSConstantBufferSetup().size()), &shaderInstance.getVSConstantBufferSetup()[0]);
            }
            if (!shaderInstance.getPSConstantBufferSetup().empty())
            {
                deviceContext->PSSetConstantBuffers(0, static_cast<uint32>(shaderInstance.getPSConstantBufferSetup().size()), &shaderInstance.getPSConstantBufferSetup()[0]);
            }

            if (!shaderInstance.getVSSRVSetup().empty())
            {
                deviceContext->VSSetShaderResources(0, static_cast<uint32>(shaderInstance.getVSSRVSetup().size()), &shaderInstance.getVSSRVSetup()[0]);
            }
            if (!shaderInstance.getPSSRVSetup().empty())
            {
                deviceContext->PSSetShaderResources(0, static_cast<uint32>(shaderInstance.getPSSRVSetup().size()), &shaderInstance.getPSSRVSetup()[0]);
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
            //technique->setupTechnique(); NOt needed any more this fucks with the shader resource we set above here

            //deviceContext->PSSetConstantBuffers(1, 1, &m_lightConstantBuffer); //this is not great and should be moved in to the shader instance creation
            if (shaderInstance.getAlphaBlend())
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
                uint32 generateVerticesCount = 6; //In the case of a particle system we assume to generate quads in the vertex shader
                if (buffer != nullptr) //if vb is null we assume the vertex shader to generate the geometry
                {
                    deviceContext->IASetInputLayout(renderInstance.getGeometryInstance().getVB()->getInputLayout());

                    deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
                    generateVerticesCount = 1; //We don't need to generate geometry in the shader based on the SV_VERTEXID so reset this to 1
                }
                else
                {
                    deviceContext->IASetInputLayout(nullptr);
                    deviceContext->IASetVertexBuffers(0, 0, nullptr, 0, 0);
                }

                deviceContext->IASetIndexBuffer(renderInstance.getGeometryInstance().getIB()->getBuffer(), DXGI_FORMAT_R32_UINT, 0);
                deviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)renderInstance.getPrimitiveType());
                deviceContext->DrawIndexed(renderInstance.getGeometryInstance().getIB()->getNumberOfIndecis() * generateVerticesCount, 0, 0);
            }
            else
            {
                deviceContext->IASetInputLayout(renderInstance.getGeometryInstance().getVB()->getInputLayout());
                deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
                deviceContext->IASetPrimitiveTopology((D3D11_PRIMITIVE_TOPOLOGY)renderInstance.getPrimitiveType());
                deviceContext->Draw((unsigned int)renderInstance.getGeometryInstance().getVB()->getVertexCount(), 0);
            }
#ifdef _DEBUG
            //if (!renderInstance.m_name.empty())
            //{
            //    pPerf->EndEvent();
            //}
#endif
            ++m_numberOfInstancePerFrame;
        }
    }

#ifdef _DEBUG
    pPerf->EndEvent();
#endif

    m_numberOfInstancesRenderingThisFrame = visibleInstances.size();
    m_totalNumberOfInstancesRendered += m_numberOfInstancesRenderingThisFrame;

    deviceContext->End(m_mainEndDrawQuery);
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
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
    D3DDebugHelperFunctions::SetDebugChildName(m_renderTargetView, "RenderSystem BackBuffer Texture for Swap Chain");
    m_backBuffer->Release();
    D3DDebugHelperFunctions::SetDebugChildName(m_renderTargetView, "RenderSystem RTV for Swap Chain");

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC depthBufferDescriptor;
    ZeroMemory(&depthBufferDescriptor, sizeof(D3D11_TEXTURE2D_DESC));
    depthBufferDescriptor.Width = windowWidth;
    depthBufferDescriptor.Height = windowHeight;
    depthBufferDescriptor.MipLevels = 1;
    depthBufferDescriptor.ArraySize = 1;
    depthBufferDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
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

    D3DDebugHelperFunctions::SetDebugChildName(m_depthStencilBuffer, "RenderSystem DepthStencil Texture");

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

    D3DDebugHelperFunctions::SetDebugChildName(m_depthStencilView, "RenderSystem DepthStencil View");

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

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void RenderSystem::cleanup()
{
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

    m_rasteriserState->Release();
    m_rasteriserWireFrameModeState->Release();
    m_depthStencilState->Release();
    m_blendState->Release();
    m_alphaBlendState->Release();
    m_samplerState->Release();

    m_beginDrawQuery->Release();
    m_endDrawQuery->Release();
    m_cubemapBeginDrawQuery->Release();
    m_cubemapEndDrawQuery->Release();
    m_shadoBeginDrawQuery->Release();
    m_shadoEndDrawQuery->Release();
    m_mainBeginDrawQuery->Release();
    m_mainEndDrawQuery->Release();

    m_timeDisjointQuery->Release();

    delete m_renderResource;
}

///-------------------------------------------------------------------------
// @brief Takes a renderlist to do ordering on the render list before actually rendering it
///-------------------------------------------------------------------------
void RenderSystem::beginDraw()
{
    PROFILE_EVENT("RenderSystem::beginDraw", Orange);
    
    ID3D11DeviceContext* deviceContext = m_deviceManager.getDeviceContext();
    deviceContext->Begin(m_timeDisjointQuery);
    deviceContext->End(m_beginDrawQuery);
    deviceContext->Begin(m_pipeLineStatistics);

    m_numberOfInstancePerFrame = 0;

    m_renderInstances.clear();

    m_cameraSystem.update(m_renderResource->m_performanceTimer->getElapsedTime(), m_renderResource->m_performanceTimer->getTime(), m_input);
    m_view = m_cameraSystem.getCamera("global")->getCamera();
    m_inverseView = m_cameraSystem.getCamera("global")->getInvCamera();

    m_messageObservers.DispatchMessages(*(m_renderResource->m_messageQueues->getRenderMessageQueue()));
    m_renderResource->m_messageQueues->getRenderMessageQueue()->reset();

    //Kick resource loading, potentially this needs to move to its own low priority thread too.
    m_resourceLoader.update();

    float clearColor[4] = { 0.8f, 0.8f, 0.8f, 0.0f }; //Reverse Z so clear to 0 instead of 1 leads to a linear depth pass
    
    deviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);
    deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.0f, 0);
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

    {
        PROFILE_EVENT("RenderSystem::beginDraw::MaterialSorting", DarkRed);
        std::sort(begin(m_renderInstances), end(m_renderInstances), [this](const RenderInstance* lhs, const RenderInstance* rhs)
        {
            const Effect* lhsEffect = m_effectCache.getEffect(lhs->getShaderInstance().getMaterial().getEffectHash());
            const Effect* rhsEffect = m_effectCache.getEffect(rhs->getShaderInstance().getMaterial().getEffectHash());
            size_t lhsTechniqueId = lhsEffect->getTechnique(lhs->getShaderInstance().getMaterial().getTechnique())->getTechniqueId();
            size_t rhsTechniqueId = rhsEffect->getTechnique(lhs->getShaderInstance().getMaterial().getTechnique())->getTechniqueId();
            return lhsTechniqueId < rhsTechniqueId; //Need a shader chache instance here to access 
        });

        std::sort(begin(m_renderInstances), end(m_renderInstances), [=](const RenderInstance* lhs, const RenderInstance* rhs)
        {
            return lhs->getShaderInstance().getAlphaBlend() < rhs->getShaderInstance().getAlphaBlend();
        });
    }
    
    RenderResourceHelper helper(m_renderResource);
    Vector3 camPos;
    PerFrameConstants perFrameConstants;
    ZeroMemory(&perFrameConstants.m_lightConstants, sizeof(LightConstants) * 8);
    LightManager& lm = helper.getWriteableResource().getLightManager();
    const CameraManager& cm = helper.getResource().getCameraManager();
    const Camera* cam = cm.getCamera("global");

    PROFILE_EVENT("RenderSystem::beginDraw::LightUpdate", DarkRed);
    //Setup light constants they might change during the frame, only support up to 8 lights for now as we do forward shading
    if (cam != nullptr)
    {
        camPos = cam->getEye();
    }

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

    deviceContext->End(m_cubemapBeginDrawQuery);
    static int counter = 0;
    if (counter > 15)
    {
        light0 = lm.getLight("light_0");
        if (light0 != nullptr)
        {
            Color diffuse = light0->getDiffuse();
            diffuse.setBlue(diffuse.b() > 1.0f ? 0.0f : diffuse.b() + 0.1f);
            diffuse.setRed(diffuse.r() < 0.0f ? 1.0f : diffuse.r() - 0.1f);
            light0->setDiffuse(diffuse);
        }

        //Loop over rts here and render to all of them
        TextureManager& tm = helper.getWriteableResource().getTextureManager();
        static size_t rtCounter = 0;
        rtCounter = (++rtCounter) % m_cubeSettings.size();
        if (m_cubeSettings[rtCounter].m_dynamic || m_cubeSettings[rtCounter].m_hasBeenRenderedTo == false)
        {
            ID3D11ShaderResourceView* srv[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
            deviceContext->PSSetShaderResources(0, 40, srv);
            m_cubeMapRenderer->initialise(m_cubeSettings[rtCounter].m_position);
            m_cubeMapRenderer->renderCubeMap(m_renderResource, const_cast<Texture*>( tm.getTexture(m_cubeSettings[rtCounter].m_texutureResourceName) ), m_renderInstances, m_deviceManager, perFrameConstants, tm);
            m_cubeSettings[rtCounter].m_hasBeenRenderedTo = true;
            deviceContext->PSSetShaderResources(0, 40, srv);
        }
        counter = 0;
    }
    ++counter;
    deviceContext->End(m_cubemapEndDrawQuery);

    deviceContext->End(m_shadoBeginDrawQuery);
    //Do shadowmap update here too to begin with
    ID3D11ShaderResourceView* srv[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
    deviceContext->PSSetShaderResources(0, 40, srv);
    m_shadowMapRenderer->renderShadowMap(m_renderResource, m_renderInstances, m_deviceManager, lm.getLight("light_1"));
    deviceContext->PSSetShaderResources(0, 40, srv);
    deviceContext->End(m_shadoEndDrawQuery);

    perFrameConstants.m_cameraPosition[0] = camPos.x();
    perFrameConstants.m_cameraPosition[1] = camPos.y();
    perFrameConstants.m_cameraPosition[2] = camPos.z();
    perFrameConstants.m_shadowMVP = m_shadowMapRenderer->getShadowMapMVP();
    deviceContext->UpdateSubresource(m_lightConstantBuffer, 0, 0, (void*)&perFrameConstants, 0, 0);
    deviceContext->VSSetConstantBuffers(0, 1, &m_lightConstantBuffer);

    CheckVisibility(m_renderInstances);

    WVPBufferContent shadowWVP = m_shadowMapRenderer->getShadowMapMVP();
    deviceContext->UpdateSubresource(m_shadowConstantBuffer, 0, 0, (void*)&shadowWVP, 0, 0);
    deviceContext->VSSetConstantBuffers(1, 1, &m_shadowConstantBuffer);
    

    //Setup shadow map SRV, since we only have one we can set it here
    ID3D11ShaderResourceView* shadowMapSRV = m_shadowMapRenderer->getShadowMap();
    deviceContext->PSSetShaderResources(33, 1, &shadowMapSRV);

}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void RenderSystem::CheckVisibility(RenderInstanceTree& renderInstances)
{
    PROFILE_EVENT("RenderSystem::CheckVisibility", Yellow);
    visibleInstances.clear();
    Frustum frustum(m_view, m_CullingProjectionMatrix);
    for (auto instance : renderInstances)
    {
        if (frustum.IsInside(instance->getBoundingBox()))
        {
            visibleInstances.push_back(instance);
        }
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void RenderSystem::endDraw()
{
    PROFILE_EVENT("RenderSystem::endDraw", Orange);
#ifdef _DEBUG
    m_debugAxis->draw(m_deviceManager, m_view, m_projection, m_renderResource);
#endif

    if (m_numberOfInstancePerFrame > 0)
    {
        HRESULT hr = m_swapChain->Present(0, 0);
        ++m_totalNumberOfRenderedFrames;
        m_averageNumberOfInstancesRenderingPerFrame = m_totalNumberOfInstancesRendered / m_totalNumberOfRenderedFrames;
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("ERROR", "Present call failed with error code: 0x%x", hr);
            MSG_TRACE_CHANNEL("ERROR", "Device removed because : 0x%x", m_deviceManager.getDevice()->GetDeviceRemovedReason());
            //assert(false);
        }
    }

    ID3D11DeviceContext* deviceContext = m_deviceManager.getDeviceContext();
    deviceContext->End(m_endDrawQuery);
    deviceContext->End(m_timeDisjointQuery);
    deviceContext->End(m_pipeLineStatistics);

    while (deviceContext->GetData(m_timeDisjointQuery, NULL, 0, 0) == S_FALSE)
    {
        Sleep(1);       // Wait a bit, but give other threads a chance to run
    }

    // Check whether timestamps were disjoint during the last frame
    D3D10_QUERY_DATA_TIMESTAMP_DISJOINT tsDisjoint;
    deviceContext->GetData(m_timeDisjointQuery, &tsDisjoint, sizeof(tsDisjoint), 0);
    if (tsDisjoint.Disjoint)
    {
        return;
    }

    UINT64 beginDrawQueryTimeStamp; //Begining of frame
    UINT64 endDrawQueryTimeStamp;
    UINT64 cubemapBeginDrawQueryTimeStamp;
    UINT64 cubemapEndDrawQueryTimeStamp;
    UINT64 shadoBeginDrawQueryTimeStamp;
    UINT64 shadoEndDrawQueryTimeStamp;
    UINT64 mainBeginDrawQueryTimeStamp;
    UINT64 mainEndDrawQueryTimeStamp;

    deviceContext->GetData(m_beginDrawQuery, &beginDrawQueryTimeStamp, sizeof(UINT64), 0);
    deviceContext->GetData(m_endDrawQuery, &endDrawQueryTimeStamp, sizeof(UINT64), 0);
    deviceContext->GetData(m_cubemapBeginDrawQuery, &cubemapBeginDrawQueryTimeStamp, sizeof(UINT64), 0);
    deviceContext->GetData(m_cubemapEndDrawQuery, &cubemapEndDrawQueryTimeStamp, sizeof(UINT64), 0);
    deviceContext->GetData(m_shadoBeginDrawQuery, &shadoBeginDrawQueryTimeStamp, sizeof(UINT64), 0);
    deviceContext->GetData(m_shadoEndDrawQuery, &shadoEndDrawQueryTimeStamp, sizeof(UINT64), 0);
    deviceContext->GetData(m_mainBeginDrawQuery, &mainBeginDrawQueryTimeStamp, sizeof(UINT64), 0);
    deviceContext->GetData(m_mainEndDrawQuery, &mainEndDrawQueryTimeStamp, sizeof(UINT64), 0);

    D3D11_QUERY_DATA_PIPELINE_STATISTICS pipeLineStats;
    ZeroMemory(&pipeLineStats, sizeof(pipeLineStats));
    deviceContext->GetData(m_pipeLineStatistics, &pipeLineStats, sizeof(pipeLineStats), 0);

    // Convert to real time
    float cubeMapBeginTime = float(cubemapBeginDrawQueryTimeStamp - beginDrawQueryTimeStamp) / float(tsDisjoint.Frequency) * 1000.0f;
    float cubeMapEndTime = float(cubemapEndDrawQueryTimeStamp - beginDrawQueryTimeStamp) / float(tsDisjoint.Frequency) * 1000.0f;

    float shadowMapBeginTime = float(shadoBeginDrawQueryTimeStamp -beginDrawQueryTimeStamp) / float(tsDisjoint.Frequency) * 1000.0f;
    float shadowMapEndTime = float(shadoEndDrawQueryTimeStamp -beginDrawQueryTimeStamp) / float(tsDisjoint.Frequency) * 1000.0f;

    float mainBeginTime = float(mainBeginDrawQueryTimeStamp - beginDrawQueryTimeStamp) / float(tsDisjoint.Frequency) * 1000.0f;
    float mainEndTime = float(mainEndDrawQueryTimeStamp - beginDrawQueryTimeStamp) / float(tsDisjoint.Frequency) * 1000.0f;

    float EndFrameTime = float(endDrawQueryTimeStamp - beginDrawQueryTimeStamp) / float(tsDisjoint.Frequency) * 1000.0f;

    MSG_TRACE_CHANNEL("RENDERSYSTEM", "Frame timings");
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "CubeMap Generation time: %f ms", cubeMapEndTime - cubeMapBeginTime);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "Shadow Generation time : %f ms", shadowMapEndTime - shadowMapBeginTime);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "Main render time       : %f ms", mainEndTime - mainBeginTime);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "Frame time             : %f ms", EndFrameTime);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "PipeLineStatistics:");
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "IAVertices: %u", pipeLineStats.IAVertices);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "IAPrimitives: %u", pipeLineStats.IAPrimitives);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "VSInvocations: %u", pipeLineStats.VSInvocations);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "GSInvocations: %u", pipeLineStats.GSInvocations);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "GSPrimitives: %u", pipeLineStats.GSPrimitives);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "CInvocations: %u", pipeLineStats.CInvocations);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "CPrimitives: %u", pipeLineStats.CPrimitives);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "PSInvocations: %u", pipeLineStats.PSInvocations);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "HSInvocations: %u", pipeLineStats.HSInvocations);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "DSInvocations: %u", pipeLineStats.DSInvocations);
    MSG_TRACE_CHANNEL("RENDERSYSTEM", "CSInvocations: %u", pipeLineStats.CSInvocations);
}

///-----------------------------------------------------------------------------
///! @brief   Match and extract all render instances that need to be rendered
///! @remark
///-----------------------------------------------------------------------------
void RenderSystem::CreateRenderList(const MessageSystem::Message& msg)
{
    UNUSEDPARAM(msg);
    const ModelManager& mm = RenderResourceHelper(m_renderResource).getResource().getModelManager();
    const MessageSystem::RenderInformation::RenderInfo* info = static_cast<const MessageSystem::RenderInformation&>(msg).GetData();

    const CreatedModel* model = mm.GetRenderResource(info->m_renderObjectid);
    if (model)
    {
        model->model->update(m_renderResource, m_renderInstances, 0.0f, info->m_world, m_view, m_projection, info->m_name);
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void RenderSystem::initialiseCubemapRendererAndResources(Resource* resource)
{
    m_cubeMapRenderer = new CubeMapRenderer(m_deviceManager, m_alphaBlendState, m_blendState);

    const Paths* path = resource->m_paths;
    auto cubeRenderSettingsFileName = path->getSettingsPath() / "cube_map_renderers.xml";
    tinyxml2::XMLDocument doc;
    if (tinyxml2::XML_NO_ERROR != doc.LoadFile(cubeRenderSettingsFileName.string().c_str()))
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
        auto elementHash = hashString(element->Value());
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

    RenderResourceHelper resourceHelper = { resource };
    TextureManager& tm = resourceHelper.getWriteableResource().getTextureManager();

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

        D3DDebugHelperFunctions::SetDebugChildName(textureResource, FormatString("RenderSystem CubeMap Render Target for cubemap no. %d", counter));

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
            D3DDebugHelperFunctions::SetDebugChildName(rtView[rtCounter], FormatString("RenderSystem CubeMap Render Target View no. %d", rtCounter));
        }

        hr = m_deviceManager.getDevice()->CreateShaderResourceView(textureResource, &srvDesc, &srView);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("CubemapRenderer_ERROR", "Failed to create shader resource view for the cubemap renderer: 0x%x", hr);
            return;
        }

        D3DDebugHelperFunctions::SetDebugChildName(srView, FormatString("RenderSystem CubeMap SRV for cubemap no. %d", counter));

        cubeMap.createRenderTarget(textureResource, rtView, srView);
        tm.addTexture(m_cubeSettings[counter].m_texutureResourceName, cubeMap);
    }
}