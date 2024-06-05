#include "Gameplay/square.h"
#include "Math/vector4.h"
#include "Math/matrix44.h"
#include "Application/BaseApplication.h"

namespace Square
{

///-----------------------------------------------------------------------------
///! @brief   Creates a square model
///! @remark
///-----------------------------------------------------------------------------
CreatedModel CreateSquare(const SquareCreationParams& params)
{
    CreatedModel square;
    RenderResource& renderResource = *(RenderResource*)params.m_resource;
    square.boundingBox = Bbox(Vector3(params.m_lowerleft.x(), params.m_lowerleft.y(), 0.0f), Vector3(params.m_upperright.x(), params.m_upperright.y(), 0.0f));

    square.model = new Model();
    auto& group = square.model->CreateMeshGroup();

    VertexDataStreams vertexStreams;
    vertexStreams.m_streams[VertexStreamType::Position] = std::vector<Vector3>();
    vertexStreams.m_streams[VertexStreamType::UVStart] = std::vector<Vector2>();

    auto& positionStream = std::get<2>(vertexStreams.m_streams[VertexStreamType::Position]);
    positionStream.push_back(Vector3(params.m_lowerleft.x(), params.m_lowerleft.y(), 0.0f));
    positionStream.push_back(Vector3(params.m_upperright.x(), params.m_lowerleft.y(), 0.0f));
    positionStream.push_back(Vector3(params.m_upperright.x(), params.m_upperright.y(), 0.0f));
    positionStream.push_back(Vector3(params.m_lowerleft.x(), params.m_upperright.y(), 0.0f));

    auto& uvStream = std::get<1>(vertexStreams.m_streams[VertexStreamType::UVStart]);
    uvStream.push_back(Vector2());
    uvStream.push_back(Vector2(1.0f, 0.0f));
    uvStream.push_back(Vector2(1.0f, 1.0f));
    uvStream.push_back(Vector2(0.0f, 1.0f));

    VertexBuffer& vb = group.GetVB();
    vb.CreateBuffer(renderResource.getDeviceManager(), *params.m_commandList, renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap(), vertexStreams);

    group.SetPrimitiveLayout((unsigned int)D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    square.model->CalculateSortKey(renderResource.getEffectCache());
    return square;
}

}
