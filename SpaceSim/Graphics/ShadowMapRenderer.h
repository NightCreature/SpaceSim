#pragma once

#include "Core/Types/Types.h"
#include "Math/Vector3.h"
#include "Graphics/Camera.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/Effect.h"
#include <d3d11.h>
#include <string>

//#ifdef _DEBUG
#include "ScreenGrab.h"
//#endif
#ifdef _DEBUG
#include <d3d11_1.h>
#endif


class DeviceManager;
class Texture;
class TextureManager;
struct PerFrameConstants;
class Light;

class ShadowMapRenderer
{
public:
    ShadowMapRenderer(DeviceManager& deviceManager, ID3D11BlendState* alphaBlendState, ID3D11BlendState* blendState, unsigned int cubeMapWidhtHeight = 1024);
    ~ShadowMapRenderer();

    void cleanup();

    void renderShadowMap(Resource* resource, const RenderInstanceTree& renderInstances, const DeviceManager& deviceManager, const Light* light);

    ID3D11ShaderResourceView* getShadowMap() { return m_shadowMapRV; }
    WVPBufferContent getShadowMapMVP() { return m_shadowMVP; }
private:

    void CheckVisibility(RenderInstanceTree& visibileInstances, const RenderInstanceTree& renderInstances);

    WVPBufferContent m_shadowMVP;
    D3D11_VIEWPORT m_lightViewPort;

    ID3D11BlendState* m_alphaBlendState;
    ID3D11BlendState* m_blendState;
    ID3D11Texture2D* m_depthStencil;
    ID3D11Texture2D* m_shadowMap;
    ID3D11DepthStencilView* m_depthStencilView;
    ID3D11RenderTargetView* m_shadowMapView;
    ID3D11ShaderResourceView* m_shadowMapRV;
    ID3D11Buffer* m_perFrameConstants;
    unsigned int m_shadowMapWidthHeight;
#ifdef _DEBUG
    ID3DUserDefinedAnnotation* pPerf;
#endif
};