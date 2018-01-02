#include "Graphics/ShaderInstance.h"

#include "Graphics/DeviceManager.h"
#include "Graphics/D3DDebugHelperFunctions.h"

#include "Graphics/texture.h"
#include "Graphics/texturemanager.h"
#include "Core/Resource/RenderResource.h"


///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
ID3D11Buffer* CreateD3DBuffer(size_t bufferSize, DeviceManager &deviceManager, const std::string &name)
{
    D3D11_BUFFER_DESC constantBufferDesc;
    ZeroMemory(&constantBufferDesc, sizeof(D3D11_BUFFER_DESC));
    constantBufferDesc.ByteWidth = static_cast<UINT>(bufferSize);
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    ID3D11Buffer* buffer = nullptr;

    HRESULT hr = deviceManager.getDevice()->CreateBuffer(&constantBufferDesc, 0, &buffer);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("EFFECT_ERROR", "failed to create the material buffer for this effect: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
        return nullptr;
    }
    D3DDebugHelperFunctions::SetDebugChildName(buffer, FormatString("%s Constant Buffer with size %u", name.c_str(), bufferSize / sizeof(float)));

    return buffer;
}


///-----------------------------------------------------------------------------
///! @brief bufferSize is in bytes, so should be the amount of floats we need for this buffer
///! @remark returns the index at which location this buffer is stored
///-----------------------------------------------------------------------------
size_t ShaderInstance::AddPsConstantBuffer(size_t bufferSize, DeviceManager& deviceManager, const std::string& name)
{
    ID3D11Buffer* buffer = CreateD3DBuffer(bufferSize, deviceManager, name);

    if (buffer != nullptr)
    {
        m_psConstantBuffers.push_back(buffer);
        return m_psConstantBuffers.size();
    }

    return static_cast<size_t>(-1);
}

///-----------------------------------------------------------------------------
///! @brief bufferSize is in bytes, so should be the amount of floats we need for this buffer
///! @remark returns the index at which location this buffer is stored
///-----------------------------------------------------------------------------
size_t ShaderInstance::AddVsConstantBuffer(size_t bufferSize, DeviceManager& deviceManager, const std::string& name)
{
    ID3D11Buffer* buffer = CreateD3DBuffer(bufferSize, deviceManager, name);
    
    if (buffer != nullptr)
    {
        m_vsConstantBuffers.push_back(buffer);
        return m_vsConstantBuffers.size();
    }

    return static_cast<size_t>(-1);
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark This is not a fix all but should at least fix the problem of the material references
///          Dynamic RT references are still a problem
///-----------------------------------------------------------------------------
void ShaderInstance::FixSrvReferences(RenderResource& resource)
{
    if (m_material.getTextureHashes().size() > m_psSvData.size())
    {
        ShaderInstanceHelpers::FixTextureSRVReferences(*this, m_material, resource);
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark 
///-----------------------------------------------------------------------------
void ShaderInstanceHelpers::FixTextureSRVReferences(ShaderInstance& shaderInstance, const Material& mat, RenderResource& renderResource)
{
    const std::vector<Material::TextureSlotMapping>& textureHashes = mat.getTextureHashes();

    size_t currentTextureIndex = 0;
    for (unsigned int counter = 0; counter < Material::TextureSlotMapping::NumSlots && currentTextureIndex < textureHashes.size(); ++counter) //We assume these are put in as order for the slots demand
    {
        const Texture* texture = nullptr;
        if (static_cast<Material::TextureSlotMapping::TextureSlot>(counter) == textureHashes[currentTextureIndex].m_textureSlot)
        {
            texture = renderResource.getTextureManager().getTexture(textureHashes[currentTextureIndex].m_textureHash);
            ++currentTextureIndex;
        }
        ID3D11ShaderResourceView* srv = texture != nullptr ? texture->getShaderResourceView() : nullptr;
        shaderInstance.AddPsSRV(srv);
    }
}