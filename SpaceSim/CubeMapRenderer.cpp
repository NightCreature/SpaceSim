#include "CubeMapRenderer.h"
#include "DeviceManager.h"
#include "Effect.h"
#include "Settings.h"
#include "SettingsManager.h"
#include <assert.h>

#include "BaseApplication.h"
#include "..\SpaceSim\RenderInstance.h"
#include "../SpaceSim/StringHelperFunctions.h"

#include "OrientationAxis.h"
#include "Types.h"
#include <d3d11.h>
#include <dxgi.h>
#include <list>
#include <directxmath.h>
#include "texturemanager.h"
#include "modelmanager.h"

#ifdef _DEBUG
#include <d3d11_1.h>
#include <atlbase.h>

#include "..\..\..\DirectXTK\Inc\ScreenGrab.h"
#endif


//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
CubeMapRenderer::CubeMapRenderer(DeviceManager& deviceManager, ID3D11BlendState* alphaBlendState, ID3D11BlendState* blendState, unsigned int cubeMapWidhtHeight /*= 1024*/):
    m_alphaBlendState(alphaBlendState),
    m_blendState(blendState)
{
    m_cubeMapWidthHeight = cubeMapWidhtHeight;

    D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
    depthStencilTextureDesc.Width = m_cubeMapWidthHeight;
    depthStencilTextureDesc.Height = m_cubeMapWidthHeight;
    depthStencilTextureDesc.MipLevels = 1;
    depthStencilTextureDesc.ArraySize = 6;
    depthStencilTextureDesc.SampleDesc.Count = 1;
    depthStencilTextureDesc.SampleDesc.Quality = 0;
    depthStencilTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilTextureDesc.CPUAccessFlags = 0;
    depthStencilTextureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
    HRESULT hr = deviceManager.getDevice()->CreateTexture2D(&depthStencilTextureDesc, NULL, &m_depthStencil);

    // Create the depth stencil view for the entire cube
    D3D11_DEPTH_STENCIL_VIEW_DESC cubeDepthStencilDescriptor;
    cubeDepthStencilDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
    cubeDepthStencilDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
    cubeDepthStencilDescriptor.Texture2DArray.FirstArraySlice = 0;
    cubeDepthStencilDescriptor.Texture2DArray.ArraySize = 1;
    cubeDepthStencilDescriptor.Texture2DArray.MipSlice = 0;
    cubeDepthStencilDescriptor.Flags = 0;
    hr = deviceManager.getDevice()->CreateDepthStencilView(m_depthStencil, &cubeDepthStencilDescriptor, &m_depthStencilView);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("CubemapRenderer_ERROR", "Failed to create depth stencil for the cubemap renderer: 0x%x", hr);
    }

    ZeroMemory(&m_cubeViewPort, sizeof(D3D11_VIEWPORT));
    m_cubeViewPort.Height = (float)m_cubeMapWidthHeight;
    m_cubeViewPort.Width = (float)m_cubeMapWidthHeight;
    m_cubeViewPort.MinDepth = 0;
    m_cubeViewPort.MaxDepth = 1;
    m_cubeViewPort.TopLeftX = 0;
    m_cubeViewPort.TopLeftY = 0;

    m_cubeProjection = math::createLeftHandedFOVPerspectiveMatrix(math::gmPI / 2.0f, 1.0f, 1.0f, 500.0f);

    D3D11_BUFFER_DESC lightContantsDescriptor;
    ZeroMemory(&lightContantsDescriptor, sizeof(D3D11_BUFFER_DESC));
    lightContantsDescriptor.ByteWidth = sizeof(LightConstants) * 8 + 4 * sizeof(float);
    lightContantsDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    hr = deviceManager.getDevice()->CreateBuffer(&lightContantsDescriptor, 0, &m_perFrameConstants);
}

CubeMapRenderer::~CubeMapRenderer()
{
}

void CubeMapRenderer::initialise(Vector3 position)
{
    m_position = position;
    createViewArray(position);
}



void CubeMapRenderer::renderCubeMap(Texture* renderTarget,const RenderInstanceTree& renderInstances, const DeviceManager& deviceManager, PerFrameConstants& perFrameConstants, const TextureManager& textureManager)
{
    ID3D11DeviceContext* deviceContext = deviceManager.getDeviceContext();
    ID3D11RenderTargetView* rtView[1] = { nullptr };
    ID3D11DepthStencilView* dsView = nullptr;
    deviceContext->OMGetRenderTargets(1, rtView, &dsView);

    D3D11_VIEWPORT OldVP;
    UINT cRT = 1;
    deviceContext->RSGetViewports( &cRT, &OldVP );
    deviceContext->RSSetViewports( 1, &m_cubeViewPort );    

    perFrameConstants.m_cameraPosition[0] = m_position.x();
    perFrameConstants.m_cameraPosition[1] = m_position.y();
    perFrameConstants.m_cameraPosition[2] = m_position.z();
    
    deviceContext->UpdateSubresource(m_perFrameConstants, 0, 0, (void*)&perFrameConstants, 0, 0);

    deviceContext->PSSetConstantBuffers(1, 1, &m_perFrameConstants);

    for (size_t rtCounter = 0; rtCounter < 6; ++rtCounter)
    {
        ID3D11RenderTargetView* aRTViews[1] = { renderTarget->getRenderTargetView(rtCounter) };
        deviceContext->OMSetRenderTargets(sizeof(aRTViews) / sizeof(aRTViews[0]), aRTViews, m_depthStencilView);
        deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0, 0);

        RenderInstanceTree::const_iterator renderInstanceIt = renderInstances.begin();
        RenderInstanceTree::const_iterator renderInstanceEnd = renderInstances.end();
        unsigned int stride = 0;
        unsigned int offset = 0;
        for (; renderInstanceIt != renderInstanceEnd; ++renderInstanceIt)
        {
            RenderInstance& renderInstance = (RenderInstance&)(*(*renderInstanceIt));

            const Effect* effect = renderInstance.getShaderInstance().getMaterial().getEffect();
            const Technique* technique = effect->getTechnique("default");
            if (technique != nullptr)
            {
                technique->setMaterialContent(deviceManager, renderInstance.getShaderInstance().getMaterial().getMaterialCB());
                WVPBufferContent wvpConstants = renderInstance.getShaderInstance().getWVPConstants();
                wvpConstants.m_projection = m_cubeProjection;
                wvpConstants.m_view = m_viewArray[rtCounter];
                technique->setWVPContent(deviceManager, wvpConstants);

                const std::vector<unsigned int>& textureHashes = renderInstance.getShaderInstance().getMaterial().getTextureHashes();
                //const std::vector<ID3D11SamplerState*>& samplerStates = renderInstance.getMaterial().getTextureSamplers();
                for (unsigned int counter = 0; counter < textureHashes.size(); ++counter)
                {
                    const Texture* texture = textureManager.getTexture(renderInstance.getShaderInstance().getMaterial().getTextureHashes()[counter]);
                    ID3D11ShaderResourceView* srv = texture->getShaderResourceView();
                    deviceContext->PSSetShaderResources(counter, 1, &srv);
                    ID3D11SamplerState* const samplerState = textureManager.getSamplerState();
                    deviceContext->PSSetSamplers(counter, 1, &samplerState);
                }

                deviceContext->VSSetShader(technique->getD3DVertexShader(), nullptr, 0);
                deviceContext->HSSetShader(technique->getD3DHullShader(), nullptr, 0);
                deviceContext->DSSetShader(technique->getD3DDomainShader(), nullptr, 0);
                deviceContext->GSSetShader(technique->getD3DGeometryShader(), nullptr, 0);
                deviceContext->PSSetShader(technique->getD3DPixelShader(), nullptr, 0);

                technique->setupTechnique(true);

                if (renderInstance.getShaderInstance().getMaterial().getBlendState())
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
                stride = vb->getVertexStride();
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
                    deviceContext->Draw(renderInstance.getGeometryInstance().getVB()->getVertexCount(), 0);
                }
            }
        }
        deviceContext->Flush();
    }

    deviceContext->OMSetRenderTargets(1, rtView, dsView);
	rtView[0]->Release();
	dsView->Release();

    deviceContext->RSSetViewports( cRT, &OldVP );
    deviceContext->GenerateMips( renderTarget->getShaderResourceView() );
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void CubeMapRenderer::createViewArray( Vector3 position )
{
    Camera viewArray[6];
    viewArray[0].positionCamera(position, position + Vector3(1.0f, 0.0f, 0.0f), Vector3::yAxis());
    viewArray[1].positionCamera(position, position + Vector3(-1.0f,0.0f, 0.0f), Vector3::yAxis());
    viewArray[2].positionCamera(position, position + Vector3(0.0f, 0.0f, 1.0f), Vector3::yAxis());
    viewArray[3].positionCamera(position, position + Vector3(0.0f, 0.0f, -1.0f), Vector3::yAxis());
    viewArray[4].positionCamera(position, position + Vector3(0.0f, 0.0f, 1.0f), Vector3::yAxis());
    viewArray[5].positionCamera(position, position + Vector3(0.0f, 0.0f, -1.0f), Vector3::yAxis());

    Matrix44 rotateX90;
    rotateX(rotateX90, math::toRadian(90.0f));

    m_viewArray[0] = viewArray[0].createCamera();
    m_viewArray[1] = viewArray[1].createCamera();
    m_viewArray[2] = viewArray[2].createCamera() * rotateX90;
    rotateX(rotateX90, math::toRadian(-90.0f));
    m_viewArray[3] = viewArray[3].createCamera() * rotateX90;
    m_viewArray[4] = viewArray[4].createCamera();
    m_viewArray[5] = viewArray[5].createCamera();
}
