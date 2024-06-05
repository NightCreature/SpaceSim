#pragma once

#include "Core/Resource/RenderResource.h"
#include "../bin/Shaders/Shaders/BindlessBuffers.h"
#include "Graphics/material.h"
#include "Graphics/texturemanager.h"

#include <vector>
#include "../RenderSystem.h"

namespace ModelHelperFunctions
{

inline void AssignTextureIndices(const RenderResource& renderResource, const std::vector<Material::TextureSlotMapping>& textureHashes, MeshResourceIndices& resourceIndices)
{
    PROFILE_FUNCTION();

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
                resourceIndices.albedoMapIndex = static_cast<uint>(heapIndex);
                break;
            case Material::TextureSlotMapping::Normals:
                resourceIndices.normalMapIndex = static_cast<uint>(heapIndex);
                break;
            case Material::TextureSlotMapping::ReflectionMap:
                resourceIndices.normalMapIndex = static_cast<uint>(heapIndex);
                break;
            case Material::TextureSlotMapping::ShadowMap:
                resourceIndices.normalMapIndex = static_cast<uint>(heapIndex);
                break;
            case Material::TextureSlotMapping::Emmisive0:
                resourceIndices.emmisiveMap = static_cast<uint>(heapIndex);
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
        resourceIndices.transformIndex = static_cast<uint>(meshGroup->CreateConstantBuffer(sizeof(WVPData), "WVPBuffer"_hash, renderResource.getDeviceManager(), renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap(), "MeshGroupWVPData"));
        resourceIndices.materialIndex = static_cast<uint>(meshGroup->CreateConstantBuffer(sizeof(MaterialConstants), "Material"_hash, renderResource.getDeviceManager(), renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap(), "MeshGroupMaterialData"));
    }
    else
    {
        MSG_ERROR_CHANNEL(__FUNCTION__, "Passed a nullptr mesh group to this function");
    }
}

inline void AssignPerSceneIndices(const RenderResource& renderResource, MeshResourceIndices& resourceIndices)
{
    const RenderSystem& renderSystem = renderResource.getRenderSystem();
    resourceIndices.sceneTransformIndex = static_cast<uint>(renderSystem.GetSceneTransformBufferIndex());
    resourceIndices.CameraBufferIndex = static_cast<uint>(renderSystem.GetCameraBufferIndex());
    const LightManager& lightManager = renderResource.getLightManager();
    resourceIndices.LightBufferIndex = static_cast<uint>(lightManager.getLightBufferIndex());
}

}