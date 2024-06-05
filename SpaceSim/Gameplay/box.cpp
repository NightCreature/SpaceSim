#include "Gameplay/box.h"
#include "Graphics/texturemanager.h"
#include "windows.h"
#include "Application/BaseApplication.h"
#include "Graphics/EffectCache.h"
#include "Core/Types/Types.h"
#include "Core/Types/TypeHelpers.h"

namespace Box
{
///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
CreatedModel CreateBox(const CreationParams& params)
{
    CreatedModel box;

    RenderResource& renderResource = *(RenderResource*)params.resource;
    Material material = params.material;

    box.model = new Model();
    if (box.model->getMeshData().empty())
    {
        VertexBuffer* vb = new VertexBuffer();
        IndexBuffer* ib = new IndexBuffer();

        //box.model->getMeshData().push_back(new MeshGroup(vb, ib, material, renderResource.getDeviceManager()));
        MeshGroup& group = box.model->CreateMeshGroup();

        std::vector<unsigned int> texCoordDim;
        if (params.m_gentexcoords)
        {
            texCoordDim.push_back(2);
        }

        constexpr static size_t nrSides = 6;
        constexpr static size_t nrVertsPerSide = 4;

        VertexDeclarationDescriptor descriptor;
        if (params.m_gentexcoords)
        {
            descriptor.textureCoordinateDimensions.push_back(2);
        }
        VertexDataStreams dataStreams = CreateDataStreams(descriptor);

        auto& positionStream = std::get<2>(dataStreams.m_streams[VertexStreamType::Position]); //Position stream
        positionStream.reserve(nrSides * nrVertsPerSide);

        const Vector3& upperRight = params.m_upperright;
        const Vector3& lowerLeft = params.m_lowerleft;

        //Front Face
        positionStream.push_back(lowerLeft);
        positionStream.push_back(Vector3(upperRight.x(), lowerLeft.y(), lowerLeft.z()));
        positionStream.push_back(Vector3(upperRight.x(), upperRight.y(), lowerLeft.z()));
        positionStream.push_back(Vector3(lowerLeft.x(), upperRight.y(), lowerLeft.z()));

        //Back face
        positionStream.push_back(Vector3(lowerLeft.x(), lowerLeft.y(), upperRight.z()));
        positionStream.push_back(Vector3(upperRight.x(), lowerLeft.y(), upperRight.z()));
        positionStream.push_back(upperRight);
        positionStream.push_back(Vector3(lowerLeft.x(), upperRight.y(), upperRight.z()));

        //Left face
        positionStream.push_back(lowerLeft);
        positionStream.push_back(Vector3(lowerLeft.x(), lowerLeft.y(), upperRight.z()));
        positionStream.push_back(Vector3(lowerLeft.x(), upperRight.y(), upperRight.z()));
        positionStream.push_back(Vector3(lowerLeft.x(), upperRight.y(), lowerLeft.z()));

        //Right face
        positionStream.push_back(Vector3(upperRight.x(), lowerLeft.y(), lowerLeft.z()));
        positionStream.push_back(Vector3(upperRight.x(), lowerLeft.y(), upperRight.z()));
        positionStream.push_back(upperRight);
        positionStream.push_back(Vector3(upperRight.x(), upperRight.y(), lowerLeft.z()));

        //Top face
        positionStream.push_back(Vector3(lowerLeft.x(), upperRight.y(), lowerLeft.z()));
        positionStream.push_back(Vector3(upperRight.x(), upperRight.y(), lowerLeft.z()));
        positionStream.push_back(upperRight);
        positionStream.push_back(Vector3(lowerLeft.x(), upperRight.y(), upperRight.z()));

        //Bottom face
        positionStream.push_back(lowerLeft);
        positionStream.push_back(Vector3(upperRight.x(), lowerLeft.y(), lowerLeft.z()));
        positionStream.push_back(Vector3(upperRight.x(), lowerLeft.y(), upperRight.z()));
        positionStream.push_back(Vector3(lowerLeft.x(), lowerLeft.y(), upperRight.z()));

        if (params.m_gentexcoords)
        {
            auto& uvStream = std::get<1>(dataStreams.m_streams[VertexStreamType::UVStart]);
            uvStream.reserve(nrSides * nrVertsPerSide);
            for (size_t counter = 0; counter < nrSides; ++counter)
            {
                uvStream.push_back(Vector2(0.0f, 0.0f));
                uvStream.push_back(Vector2(1.0f, 0.0f));
                uvStream.push_back(Vector2(1.0f, 1.0f));
                uvStream.push_back(Vector2(0.0f, 1.0f));
            }
        }

        VertexDeclarationDescriptor vertexDesc;
        vertexDesc.textureCoordinateDimensions = texCoordDim;

        vb->CreateBuffer(renderResource.getDeviceManager(), *params.m_commandList, renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap(), dataStreams);

        constexpr unsigned int indexData[] =
        {
            0, 1, 2, 2, 3, 0, //FF
            4, 5, 6, 6, 7, 4, //BF
            8, 9, 10, 10, 11, 8, //SF1
            12, 13, 14, 14, 15, 12, //SF2
            16, 17, 18, 18, 19, 16, //VF1
            20, 21, 22, 22, 23, 20 //VF2
        };

        ib->Create(renderResource.getDeviceManager(), *params.m_commandList, sizeof(indexData), (void*)&indexData[0]);
        ib->setNumberOfIndecis(36);

        group.SetPrimitiveLayout(static_cast<uint32>(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }
    else
    {
        //Should probably update the material here althoguh that is not how this should be used to begin with
    }

    box.model->CalculateSortKey(renderResource.getEffectCache());
    return box;
}

}