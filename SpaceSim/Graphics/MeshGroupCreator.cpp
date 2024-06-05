#include "Graphics/MeshGroupCreator.h"

#include "Graphics/EffectCache.h"
#include "Graphics/Model/MeshHelperFunctions.h"
#include "Graphics/ShaderCache.h"
#include "Graphics/Texture.h"
#include "Graphics/texturemanager.h"

#include "assert.h"
#include <span>

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
CreatedMeshGroup MeshGroupCreator::CreateMeshGroup(const CreationParams& params)
{
    CreatedMeshGroup meshGroup;
    RenderResource& renderResource = RenderResourceHelper(params.m_resource).getWriteableResource();
    meshGroup.meshGroup = new MeshGroup(params.mat);

    VertexBuffer& vb = meshGroup.meshGroup->GetVB();
    IndexBuffer& ib = meshGroup.meshGroup->GetIB();

    MeshResourceIndices& resourceIndices = meshGroup.meshGroup->GetResourceInices();
    resourceIndices = vb.CreateBuffer(renderResource.getDeviceManager(), *params.m_commandList, renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap(), params.m_dataStream);

    ib.setNumberOfIndecis((unsigned int)params.m_indices.size());
    ib.Create(renderResource.getDeviceManager(), *params.m_commandList, (unsigned int)params.m_indices.size() * sizeof(unsigned int), (void*)&params.m_indices[0]);


    //Add a binding to the shadow map
    //shaderInstance.AddPsSRV(m_shadowMapRenderer->getShadowMap());

    //Setup material and constant indices
    ModelHelperFunctions::AssignTextureIndices(renderResource, params.mat.getTextureHashes(), resourceIndices);
    ModelHelperFunctions::CreateConstantBuffers(resourceIndices, meshGroup.meshGroup, renderResource);

    return meshGroup;
}

void MeshGroupCreator::normalizeNormals(std::vector<Vector3>& normals)
{
    for (size_t i = 0; i < normals.size(); i++)
    {
        normals[i].normalize();
    }
}