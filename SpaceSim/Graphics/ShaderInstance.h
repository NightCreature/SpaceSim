#pragma once 

#include "Core/Types/Types.h"

#include "Graphics/material.h"

#include <string>

struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

class DeviceManager;
class Effect;

class ShaderInstance
{
public:
    ShaderInstance() {}
    ~ShaderInstance() {}

    //Add get set operations here
    const std::vector<ID3D11Buffer*>& getPSConstantBufferSetup() const { return m_psConstantBuffers; }
    const std::vector<ID3D11Buffer*>& getVSConstantBufferSetup() const { return m_vsConstantBuffers; }
    const std::vector<ID3D11ShaderResourceView*>& getPSSRVSetup() const { return m_psSvData; }
    const std::vector<ID3D11ShaderResourceView*>& getVSSRVSetup() const { return m_vsSvData; }

    size_t AddPsConstantBuffer(size_t bufferSize, DeviceManager& deviceManager, const std::string& name);
    size_t AddVsConstantBuffer(size_t bufferSize, DeviceManager& deviceManager, const std::string& name);

    void AddPsSRV(ID3D11ShaderResourceView* SRV) { m_psSvData.push_back(SRV); }

    const Effect* getEffect() const { return m_effect; }
    uint32 getTechniqueHash() const { return m_techniqueHash; }
    bool getAlphaBlend() const { return m_alphaBlendEnabled; }
    const Material& getMaterial() const { return m_material; }
    void setMaterial(const Material& mat) { m_material = mat; }
private:
    //This is pure render information 
    Material m_material;
    Effect* m_effect;
    //These only contain buffers beyond basic setup for the effect
    std::vector<ID3D11Buffer*> m_psConstantBuffers;
    std::vector<ID3D11Buffer*> m_vsConstantBuffers;
    std::vector<ID3D11ShaderResourceView*> m_psSvData;
    std::vector<ID3D11ShaderResourceView*> m_vsSvData;
    uint32 m_techniqueHash;
    bool m_alphaBlendEnabled;
};

namespace ShaderInstanceHelpers
{
    void FixTextureSRVReferences(ShaderInstance& shaderInstance, const Material& mat, RenderResource& renderResource);
}