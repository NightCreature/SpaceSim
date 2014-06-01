#include "OrientationAxis.h"

#include "BaseApplication.h"
#include "DeviceManager.h"
#include "EffectCache.h"

#include "tinyxml2.h"

#include <d3d11.h>
#include <assert.h>

OrientationAxis::LineVertex OrientationAxis::m_vertices[] = 
{//		x		y	  z	
    {-1000.0f, 0.0f, 0.0f},//, 0xFFFFFFFF
    {1000.0f , 0.0f, 0.0f},//, 0xFFFFFFFF
    {0.0f, -1000.0f, 0.0f},//, 0xFFFFFFFF
    {0.0f, 1000.0f , 0.0f},//, 0xFFFFFFFF
    {0.0f, 0.0f, -1000.0f},//, 0xFFFFFFFF
    {0.0f, 0.0f, 1000.0f },//, 0xFFFFFFFF
    //Add lines for plus one on each axis
    {1.0f,  0.0f, 0.0f},//, 0xFFFFFFFF
    {1.0f, 0.25f, 0.0f},//, 0xFFFFFFFF
    {0.0f,  1.0f, 0.0f},//, 0xFFFFFFFF
    {0.25f, 1.0f, 0.0f},//, 0xFFFFFFFF
    {0.0f,  0.0f, 1.0f},//, 0xFFFFFFFF
    {0.0f, 0.25f, 1.0f} //, 0xFFFFFFFF
};
//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void OrientationAxis::initialise(Resource* resource, const DeviceManager& deviceManger)
{
    GameResourceHelper helper(resource);
    m_effect = const_cast<Effect*>(helper.getWritableGameResource()->getEffectCache()->createEffect(resource, "Shaders\\Effects\\Debug_Color_Shader.xml"));

    unsigned int bufferSize = 12 * sizeof(LineVertex);

    //Move pointer to start of vertex array
    const Technique* technique = m_effect->getTechnique("default");
    VertexDecalartionDesctriptor vertexDesc;
    m_vertexBuffer.createBufferAndLayoutElements(deviceManger, bufferSize, m_vertices, false, vertexDesc, technique->getVertexShader()->getShaderBlob());
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void OrientationAxis::draw( const DeviceManager& deviceManager )
{   
    transform(deviceManager); //Needs to move to the update of an object not the draw step
    ID3D11DeviceContext* deviceContext = deviceManager.getDeviceContext();
    const Technique* technique = m_effect->getTechnique("default");
    technique->setWVPContent(deviceManager, m_wvpConstants);

    deviceContext->VSSetShader(technique->getD3DVertexShader(), nullptr, 0);
    deviceContext->HSSetShader(technique->getD3DHullShader(), nullptr, 0);
    deviceContext->DSSetShader(technique->getD3DDomainShader(), nullptr, 0);
    deviceContext->GSSetShader(technique->getD3DGeometryShader(), nullptr, 0);
    deviceContext->PSSetShader(technique->getD3DPixelShader(), nullptr, 0);

    technique->setupTechnique();

    // Set vertex buffer stride and offset.
    unsigned int offset = 0;
    unsigned int stride = m_vertexBuffer.getVertexStride();
    ID3D11Buffer* buffer = m_vertexBuffer.getBuffer();
    deviceContext->IASetInputLayout( m_vertexBuffer.getInputLayout() );
    deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    deviceContext->Draw(12, 0);
}
//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void OrientationAxis::transform(const DeviceManager& deviceManager)
{
    m_wvpConstants.m_projection = Application::m_projection;
    m_wvpConstants.m_view = Application::m_view;
    Matrix44 world;
    world.identity();
    m_wvpConstants.m_world = world; 
    UNUSEDPARAM(deviceManager);
}