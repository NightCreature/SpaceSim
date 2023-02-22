#pragma once

#include "Core/Resource/RenderResource.h"
#include "../bin/Shaders/Shaders/BindlessBuffers.h"
#include "Graphics/material.h"
#include "Graphics/texturemanager.h"

#include <vector>

namespace ModelHelperFunctions
{

inline void AssignTextureIndices(const RenderResource& renderResource, const std::vector<Material::TextureSlotMapping>& textureHashes, MeshResourceIndices& resourceIndices)
{
    const auto& textureManager = renderResource.getTextureManager();
    for (const auto& textureSlotMapping : textureHashes)
    {
        const TextureInfo* textureInf0 = textureManager.getTexture(textureSlotMapping.m_textureHash);
        if (textureInf0)
        {
            auto heapIndex = textureInf0->m_heapIndex;
            switch (textureSlotMapping.m_textureSlot)
            {
            case Material::TextureSlotMapping::Diffuse0:
                resourceIndices.albedoMapIndex = heapIndex;
                break;
            case Material::TextureSlotMapping::Normals:
                resourceIndices.normalMapIndex = heapIndex;
                break;
            case Material::TextureSlotMapping::ReflectionMap:
                resourceIndices.normalMapIndex = heapIndex;
                break;
            case Material::TextureSlotMapping::ShadowMap:
                resourceIndices.normalMapIndex = heapIndex;
                break;
            default:
                MSG_TRACE_CHANNEL(__FUNCTION__, "Didn't handle texture slot: %d", textureSlotMapping.m_textureSlot);
                break;
            }
        }
    }
}

inline void CreateConstantBuffers(MeshResourceIndices& resourceIndices, MeshGroup* meshGroup, RenderResource& renderResource)
{
    //By default we have 2 constant buffers the material and Per instance data
    if (meshGroup != nullptr)
    {
        resourceIndices.transformIndex = meshGroup->CreateConstantBuffer(sizeof(WVPBufferContent), "WVPBuffer"_hash, renderResource.getDeviceManager(), renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap());
        resourceIndices.materialIndex = meshGroup->CreateConstantBuffer(sizeof(MaterialContent), "Material"_hash, renderResource.getDeviceManager(), renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap());
    }
    else
    {
        MSG_ERROR_CHANNEL(__FUNCTION__, "Passed a nullptr mesh group to this function");
    }
}

}