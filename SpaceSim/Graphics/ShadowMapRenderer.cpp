#include "Graphics/ShadowMapRenderer.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/Effect.h"
#include "Core/Settings/Settings.h"
#include "Core/Settings/SettingsManager.h"
#include <assert.h>

#include "Application/BaseApplication.h"
#include "Graphics/RenderInstance.h"
#include "Core/StringOperations/StringHelperFunctions.h"

#include "Graphics/OrientationAxis.h"
#include "Core/Types/Types.h"
#include <d3d11.h>
#include <dxgi.h>
#include <list>
#include <directxmath.h>
#include "Graphics/texturemanager.h"
#include "Graphics/modelmanager.h"
#include "Graphics/light.h"
#include "Core/Profiler/ProfilerMacros.h"

#include <numeric>

#include "Graphics/Frustum.h"
#include "Graphics/D3DDebugHelperFunctions.h"

#ifdef _DEBUG
#include <d3d11_1.h>

#include "ScreenGrab.h"
#endif

static const size_t shadowMapTechniqueHash = "shadow_map"_hash;

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
ShadowMapRenderer::ShadowMapRenderer(DeviceManager& deviceManager, ID3D11BlendState* alphaBlendState, ID3D11BlendState* blendState, unsigned int shadowMapWidhtHeight /*= 1024*/) :
    m_blendState(blendState),
    m_alphaBlendState(alphaBlendState)
 {
     m_shadowMapWidthHeight = shadowMapWidhtHeight;
     m_shadowMapWidthHeight = 2048;

     D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
     depthStencilTextureDesc.Width = m_shadowMapWidthHeight;
     depthStencilTextureDesc.Height = m_shadowMapWidthHeight;
     depthStencilTextureDesc.MipLevels = 1;
     depthStencilTextureDesc.ArraySize = 1;
     depthStencilTextureDesc.SampleDesc.Count = 1;
     depthStencilTextureDesc.SampleDesc.Quality = 0;
     depthStencilTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
     depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
     depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
     depthStencilTextureDesc.CPUAccessFlags = 0;
     depthStencilTextureDesc.MiscFlags = 0;
     HRESULT hr = deviceManager.getDevice()->CreateTexture2D(&depthStencilTextureDesc, NULL, &m_depthStencil);
     if (FAILED(hr))
     {
         MSG_TRACE_CHANNEL("CubemapRenderer_ERROR", "Failed to create depth stencil for the cubemap renderer: 0x%x", hr);
     }

     D3DDebugHelperFunctions::SetDebugChildName(m_depthStencil, "ShadowMapRenderer DepthStencil Texture");

     // Create the depth stencil view
     D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescriptor;
     ZeroMemory(&depthStencilViewDescriptor, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
     depthStencilViewDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
     depthStencilViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
     depthStencilViewDescriptor.Texture2D.MipSlice = 0;
     if (FAILED(deviceManager.getDevice()->CreateDepthStencilView(m_depthStencil, &depthStencilViewDescriptor, &m_depthStencilView)))
     {
         MSG_TRACE_CHANNEL("ERROR", "Failed to create the depth stencil view")
             return;
     }
     D3DDebugHelperFunctions::SetDebugChildName(m_depthStencilView, "ShadowMapRenderer DepthStencil View");

     D3D11_TEXTURE2D_DESC shadowMapTextureDesc;
     shadowMapTextureDesc.Width = m_shadowMapWidthHeight;
     shadowMapTextureDesc.Height = m_shadowMapWidthHeight;
     shadowMapTextureDesc.MipLevels = 1;
     shadowMapTextureDesc.ArraySize = 1;
     shadowMapTextureDesc.SampleDesc.Count = 1;
     shadowMapTextureDesc.SampleDesc.Quality = 0;
     shadowMapTextureDesc.Format = DXGI_FORMAT_R32_FLOAT;
     shadowMapTextureDesc.Usage = D3D11_USAGE_DEFAULT;
     shadowMapTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
     shadowMapTextureDesc.CPUAccessFlags = 0;
     shadowMapTextureDesc.MiscFlags = 0;
     hr = deviceManager.getDevice()->CreateTexture2D(&shadowMapTextureDesc, NULL, &m_shadowMap);
     if (FAILED(hr))
     {
         MSG_TRACE_CHANNEL("CubemapRenderer_ERROR", "Failed to create depth stencil for the cubemap renderer: 0x%x", hr);
     }
     D3DDebugHelperFunctions::SetDebugChildName(m_shadowMap, "ShadowMapRenderer ShadowMap Texture");

     // Create the depth stencil view
     D3D11_RENDER_TARGET_VIEW_DESC shadowMapViewDescriptor;
     ZeroMemory(&shadowMapViewDescriptor, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
     shadowMapViewDescriptor.Format = DXGI_FORMAT_R32_FLOAT;
     shadowMapViewDescriptor.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
     shadowMapViewDescriptor.Texture2D.MipSlice = 0;
     if (FAILED(deviceManager.getDevice()->CreateRenderTargetView(m_shadowMap, &shadowMapViewDescriptor, &m_shadowMapView)))
     {
         MSG_TRACE_CHANNEL("ERROR", "Failed to create the depth stencil view")
         return;
     }
     D3DDebugHelperFunctions::SetDebugChildName(m_shadowMapView, "ShadowMapRenderer ShadowMap RTV");

     D3D11_SHADER_RESOURCE_VIEW_DESC shadowMapSRVDescriptor;
     ZeroMemory(&shadowMapSRVDescriptor, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
     shadowMapSRVDescriptor.Format = DXGI_FORMAT_R32_FLOAT;
     shadowMapSRVDescriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
     shadowMapSRVDescriptor.Texture2D.MipLevels = 1;
     if (FAILED(deviceManager.getDevice()->CreateShaderResourceView(m_shadowMap, &shadowMapSRVDescriptor, &m_shadowMapRV)))
     {
         MSG_TRACE_CHANNEL("ERROR", "Failed to create the depth stencil view")
         return;
     }
     D3DDebugHelperFunctions::SetDebugChildName(m_shadowMapRV, "ShadowMapRenderer ShadowMap SRV");

     ID3D11Device* device = deviceManager.getDevice();
     D3D11_BUFFER_DESC wvpBufferDescriptor;
     ZeroMemory(&wvpBufferDescriptor, sizeof(D3D11_BUFFER_DESC));
     wvpBufferDescriptor.ByteWidth = sizeof(float) * 16 * 3;
     wvpBufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
     if (FAILED(device->CreateBuffer(&wvpBufferDescriptor, 0, &m_perFrameConstants)))
     {
         MSG_TRACE_CHANNEL("EFFECT_ERROR", "failed to create the World, View and Porjection buffer for this effect");
         return;
     }
     D3DDebugHelperFunctions::SetDebugChildName(m_perFrameConstants, "ShadowMapRendererPer Frame Constants");

     ZeroMemory(&m_lightViewPort, sizeof(D3D11_VIEWPORT));
     m_lightViewPort.Height = (float)m_shadowMapWidthHeight;
     m_lightViewPort.Width = (float)m_shadowMapWidthHeight;
     m_lightViewPort.MinDepth = 0;
     m_lightViewPort.MaxDepth = 1;
     m_lightViewPort.TopLeftX = 0;
     m_lightViewPort.TopLeftY = 0;

     m_shadowMVP.m_projection = math::createLeftHandedFOVPerspectiveMatrix(math::gmPI * 0.5f, 1.0f, 500.0f, 0.1f);
#ifdef _DEBUG
     hr = deviceManager.getDeviceContext()->QueryInterface(__uuidof(pPerf), reinterpret_cast<void**>(&pPerf));
#endif

}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
ShadowMapRenderer::~ShadowMapRenderer()
{

}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ShadowMapRenderer::cleanup()
{
    m_depthStencil->Release();
    m_shadowMap->Release();
    m_depthStencilView->Release();
    m_shadowMapView->Release();
    m_shadowMapRV->Release();
    m_perFrameConstants->Release();

#ifdef _DEBUG
    pPerf->Release();
#endif
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void ShadowMapRenderer::CheckVisibility(RenderInstanceTree& visibileInstances, const RenderInstanceTree& renderInstances)
{
    PROFILE_EVENT("RenderSystem::CheckVisibility", Yellow);
    visibileInstances.clear();
    Frustum frustum(m_shadowMVP.m_view, m_shadowMVP.m_projection);
    for (auto instance : renderInstances)
    {
        if (frustum.IsInside(instance->getBoundingBox()))
        {
            visibileInstances.push_back(instance);
        }
    }
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void ShadowMapRenderer::renderShadowMap(Resource* resource, const RenderInstanceTree& renderInstances, const DeviceManager& deviceManager, const Light* light)
{

    UNUSEDPARAM(resource);
    UNUSEDPARAM(renderInstances);
    UNUSEDPARAM(deviceManager);
    UNUSEDPARAM(light);

    /*PROFILE_EVENT("ShadowMapRenderer::renderShadowMap", Blue);
    if (light != nullptr)
    {
#ifdef _DEBUG
        pPerf->BeginEvent(L"ShadowMapRenderer::renderShadowMap");
#endif

        ID3D11DeviceContext* deviceContext = deviceManager.getDeviceContext();
        ID3D11RenderTargetView* rtView[1] = { nullptr };
        ID3D11DepthStencilView* dsView = nullptr;
        deviceContext->OMGetRenderTargets(1, rtView, &dsView);

        D3D11_VIEWPORT OldVP;
        UINT cRT = 1;
        deviceContext->RSGetViewports(&cRT, &OldVP);
        deviceContext->RSSetViewports(1, &m_lightViewPort);

        deviceContext->OMSetRenderTargets(1, &m_shadowMapView, m_depthStencilView);

        float clearColor[4] = { std::numeric_limits<float>::max(), 1.0f, 1.0f, 1.0f };
        deviceContext->ClearRenderTargetView(m_shadowMapView, clearColor);
        deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 0.0f, 0);

        RenderInstanceTree visibleRenderInstances;
        visibleRenderInstances.reserve(renderInstances.size());
        CheckVisibility(visibleRenderInstances, renderInstances);

        RenderInstanceTree::const_iterator renderInstanceIt = visibleRenderInstances.begin();
        RenderInstanceTree::const_iterator renderInstanceEnd = visibleRenderInstances.end();
        unsigned int stride = 0;
        unsigned int offset = 0;

        RenderResourceHelper gameResource = { resource };
        const ShaderCache& shaderCache = gameResource.getResource().getShaderCache();
        UNUSEDPARAM(shaderCache);
        const EffectCache& effectCache = gameResource.getResource().getEffectCache();

        size_t oldTechniqueId = 0;
        std::vector<ID3D11ShaderResourceView*> resourceViews;
        std::vector<ID3D11SamplerState*> samplerStates;
        resourceViews.reserve(128);
        samplerStates.reserve(128);

        Camera cam;
        cam.positionCamera(light->getPosition(), light->getPosition() + light->getDirection(), Vector3::yAxis());
        m_shadowMVP.m_view = cam.createCamera();

        //UNUSEDPARAM(light);
        //wvpContent.m_view = Application::m_view;
        for (; renderInstanceIt != renderInstanceEnd; ++renderInstanceIt)
        {
            resourceViews.clear();
            samplerStates.clear();

            RenderInstance& renderInstance = (RenderInstance&)(*(*renderInstanceIt));
#ifdef _DEBUG
            if (!renderInstance.m_name.empty())
            {
                pPerf->BeginEvent(renderInstance.m_name.c_str());
            }
#endif

            const auto shaderInstance = renderInstance.getShaderInstance();
            const Effect* effect = effectCache.getEffect(shaderInstance.getMaterial().getEffectHash());
            const Technique* technique = effect->getTechnique(shaderInstance.getTechniqueHash());

            if (technique)
            {
                //Special bit for shadow map rendering we need
                if (!shaderInstance.getVSConstantBufferSetup().empty())
                {
                    deviceContext->VSSetConstantBuffers(0, static_cast<uint32>(shaderInstance.getVSConstantBufferSetup().size()), &shaderInstance.getVSConstantBufferSetup()[0]);
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

                //m_shadowMVP.m_world = renderInstance.getShaderInstance().getWVPConstants().m_world;
                //deviceManager.getDeviceContext()->UpdateSubresource(m_perFrameConstants, 0, 0, (void*)&m_shadowMVP, 0, 0);

                if (technique->getTechniqueId() != oldTechniqueId)
                {
                    //this will crash, also we shouldnt set this if the shader id hasnt changed from the previous set
                    //deviceContext->VSSetShader(shaderCache.getVertexShader(technique->getVertexShader()) ? shaderCache.getVertexShader(technique->getVertexShader())->getShader() : nullptr, nullptr, 0);
                    //deviceContext->HSSetShader(shaderCache.getHullShader(technique->getHullShader()) ? shaderCache.getHullShader(technique->getHullShader())->getShader() : nullptr, nullptr, 0);
                    //deviceContext->DSSetShader(shaderCache.getDomainShader(technique->getDomainShader()) ? shaderCache.getDomainShader(technique->getDomainShader())->getShader() : nullptr, nullptr, 0);
                    //deviceContext->GSSetShader(shaderCache.getGeometryShader(technique->getGeometryShader()) ? shaderCache.getGeometryShader(technique->getGeometryShader())->getShader() : nullptr, nullptr, 0);
                    //deviceContext->PSSetShader(shaderCache.getPixelShader(technique->getPixelShader()) ? shaderCache.getPixelShader(technique->getPixelShader())->getShader() : nullptr, nullptr, 0);
                    oldTechniqueId = technique->getTechniqueId();
                }
                technique->setupTechnique();

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
            }
#ifdef _DEBUG
            pPerf->EndEvent();
#endif
        }

        deviceContext->Flush();

        deviceContext->OMSetRenderTargets(1, rtView, dsView);
        rtView[0]->Release();
        dsView->Release();

        deviceContext->RSSetViewports(cRT, &OldVP);

#ifdef _DEBUG
        pPerf->EndEvent();
#endif
    }
    */
}
