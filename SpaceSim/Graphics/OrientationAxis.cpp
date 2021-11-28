#include "Graphics/OrientationAxis.h"

#include "Application/BaseApplication.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/EffectCache.h"

#include "Core/tinyxml2.h"

#include <d3d11.h>
#include <assert.h>
#include "D3D12/CommandQueue.h"

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

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void OrientationAxis::cleanup()
{
    m_vertexBuffer.cleanup();
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void OrientationAxis::initialise(Resource* resource, const DeviceManager& deviceManger)
{
    RenderResourceHelper helper(resource);
    unsigned int bufferSize = 12 * sizeof(LineVertex);

    auto& commandQueue = helper.getWriteableResource().getDeviceManager().GetCommandQueue(helper.getResource().getResourceLoader().m_uploadQueueHandle);
    auto& commandList = commandQueue.GetCommandList(helper.getResource().getResourceLoader().m_currentUploadCommandListHandle);

    //Move pointer to start of vertex array
    VertexDeclarationDescriptor vertexDesc;
    m_vertexBuffer.Create(deviceManger, commandList, bufferSize, m_vertices, vertexDesc.GetVertexStride());
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void OrientationAxis::draw( const DeviceManager& deviceManager, const Matrix44& view, const Matrix44& projection, Resource* resource )
{   

    UNUSEDPARAM(resource);
    UNUSEDPARAM(deviceManager);
    UNUSEDPARAM(view);
    UNUSEDPARAM(projection);
    //UNUSEDPARAM(resource);
    //transform(deviceManager, view, projection); //Needs to move to the update of an object not the draw step
    //ID3D11DeviceContext* deviceContext = deviceManager.getDeviceContext();
    ////const Technique* technique = m_effect->getTechnique("default");
    ////technique->setWVPContent(deviceManager, m_wvpConstants);

    ////RenderResourceHelper renderResource = RenderResourceHelper(resource);
    ////const ShaderCache& shaderCache = renderResource.getResource().getShaderCache();
    //////this will crash, also we shouldnt set this if the shader id hasnt changed from the previous set
    ////deviceContext->VSSetShader(shaderCache.getVertexShader(technique->getVertexShader()) ? shaderCache.getVertexShader(technique->getVertexShader())->getShader() : nullptr, nullptr, 0);
    ////deviceContext->HSSetShader(shaderCache.getHullShader(technique->getHullShader()) ? shaderCache.getHullShader(technique->getHullShader())->getShader() : nullptr, nullptr, 0);
    ////deviceContext->DSSetShader(shaderCache.getDomainShader(technique->getDomainShader()) ? shaderCache.getDomainShader(technique->getDomainShader())->getShader() : nullptr, nullptr, 0);
    ////deviceContext->GSSetShader(shaderCache.getGeometryShader(technique->getGeometryShader()) ? shaderCache.getGeometryShader(technique->getGeometryShader())->getShader() : nullptr, nullptr, 0);
    ////deviceContext->PSSetShader(shaderCache.getPixelShader(technique->getPixelShader()) ? shaderCache.getPixelShader(technique->getPixelShader())->getShader() : nullptr, nullptr, 0);

    ////technique->setupTechnique();

    //// Set vertex buffer stride and offset.
    //unsigned int offset = 0;
    //unsigned int stride = static_cast<unsigned int>(m_vertexBuffer.getVertexStride());
    //ID3D11Buffer* buffer = m_vertexBuffer.getBuffer();
    //deviceContext->IASetInputLayout( m_vertexBuffer.getInputLayout() );
    //deviceContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
    //deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    //deviceContext->Draw(12, 0);
}
///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void OrientationAxis::transform(const DeviceManager& deviceManager, const Matrix44& view, const Matrix44& projection)
{
    m_wvpConstants.m_projection = projection;
    m_wvpConstants.m_view = view;
    Matrix44 world;
    world.identity();
    m_wvpConstants.m_world = world; 
    UNUSEDPARAM(deviceManager);
}