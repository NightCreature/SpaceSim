#pragma once
#include "Core/Types/Types.h"
#include "Math/Vector3.h"
#include "Graphics/Camera.h"
#include "Graphics/DeviceManager.h"
#include <d3d11.h>
#include <string>

//#ifdef _DEBUG
#include "ScreenGrab.h"
//#endif

class DeviceManager;
class Texture;
class TextureManager;
struct PerFrameConstants;

class CubeMapRenderer
{
public:
    CubeMapRenderer(DeviceManager& deviceManager, ID3D11BlendState* alphaBlendState, ID3D11BlendState* blendState, unsigned int cubeMapWidhtHeight = 1024);
    ~CubeMapRenderer();

    void cleanup();

    void initialise(Vector3 position);
    void CheckVisibility(RenderInstanceTree& visibleRenderInstances, const RenderInstanceTree& renderInstances, const Matrix44& viewMatrix);
    void createViewArray(Vector3 position);
    void renderCubeMap(Resource* resource, Texture* renderTarget, const RenderInstanceTree& renderInstances, const DeviceManager& deviceManager, PerFrameConstants& perFrameConstants, const TextureManager& textureManager);
private:
    Matrix44 createCamera();

    Matrix44 m_viewArray[6];
    Matrix44 m_cubeProjection;  
    D3D11_VIEWPORT m_cubeViewPort;
    Vector3 m_position;

    ID3D11BlendState* m_alphaBlendState;
    ID3D11BlendState* m_blendState;
    ID3D11Texture2D* m_depthStencil;
    ID3D11DepthStencilView* m_depthStencilView;
    ID3D11Buffer* m_perFrameConstants;
    unsigned int m_cubeMapWidthHeight;
};

