#pragma once

#include "Core/Resource/RenderResource.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/D3D12/ConstantBuffer.h"
#include "Graphics/D3D12/DeviceManagerD3D12.h"
#include "Graphics/D3D12/PipelineObject.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/ShaderDataStructures.h"
#include "Graphics/Shaders.h"
#include "Math/matrix44.h"

#include <D3Dcompiler.h>
#include <d3d11.h>
#include <map>
#include <string>

class Resource;

namespace tinyxml2
{
    class XMLElement;
}

class Technique
{
public:
    Technique(Resource* resource) : 
        m_resource(resource)
        {}
    ~Technique() {}

    void cleanup() 
    {
    }

    void deserialise(const tinyxml2::XMLElement* element);

    size_t getNameHash() const { return m_nameHash; }

    void setWVPContent(const DeviceManager& deviceManager, const WVPBufferContent& wvpContent) const;
    void setMaterialContent(const DeviceManager& deviceManager, const MaterialContent& materialContent) const;

    //ID3D11VertexShader* getD3DVertexShader() const { return m_vertexShader ? m_vertexShader->getShader() : nullptr; }
    //ID3D11HullShader* getD3DHullShader() const { return m_hullShader ? m_hullShader->getShader() : nullptr; }
    //ID3D11DomainShader* getD3DDomainShader() const { return m_domainShader ? m_domainShader->getShader() : nullptr; }
    //ID3D11GeometryShader* getD3DGeometryShader() const { return m_geometryShader ? m_geometryShader->getShader() : nullptr; }
    //ID3D11PixelShader* getD3DPixelShader() const { return m_pixelShader ? m_pixelShader->getShader() : nullptr; }
    //ID3D11ComputeShader* getD3DComputeShader() const { return m_computeShader ? m_computeShader->getShader() : nullptr; }

    void setupTechnique(bool alsoUseOnGS = false) const
    {
        UNUSEDPARAM(alsoUseOnGS);
    }

    size_t getTechniqueId() const { return m_techniqueId; }
    const PipelineObject& GetPipelineState() const { return m_pso; }
    PipelineObject& GetPipelineState() { return m_pso; }

    HASH_ELEMENT_DEFINITION(Technique);
private:
#ifdef _DEBUG
    std::string m_name; //This should be compiled out in release
#endif
    PipelineObject m_pso; //This might bind the actual shaders already
    size_t m_techniqueId;
    size_t m_nameHash;
    Resource* m_resource;
public: //TODO remove
    //ID3D11Buffer* m_constantBuffers[2]; //Contains the projection, view and world matrices, these should move in to something called constant buffer which can be set by the model
    ShaderParameters m_shaderParameters; //We only know this for root variables

    //This needs to know about the Parameters it has, but if I am going to rewrite this maybe this should just have a heap that the stuff is in so we can just bind that and forget about setting this properly
};

class Effect
{
public:
    Effect() {}
    ~Effect(void) 
    {
        //if (m_vertexShaderBlob)
        //{
        //    m_vertexShaderBlob->Release();
        //    m_vertexShaderBlob = 0;
        //}

        //if (m_vertexShader)
        //{
        //    m_vertexShader->Release();
        //    m_vertexShader = 0;
        //}

        //if (m_pixelShader)
        //{
        //    m_pixelShader->Release();
        //    m_pixelShader = 0;
        //}

        //if (m_wvpBuffer)
        //{
        //    m_wvpBuffer->Release();
        //    m_wvpBuffer = 0;
        //}
    }

    void cleanup() { for (auto technique : m_techniques) { technique.second.cleanup(); } }

    void deserialise(const tinyxml2::XMLElement* node, Resource* resource);
    void setupEffect() const;

    const Technique* getTechnique(const std::string& techniqueName) const;
    const Technique* getTechnique(const size_t techniqueName) const;

    Technique* getTechnique(const std::string& techniqueName);
    Technique* getTechnique(const size_t techniqueName);
    HASH_ELEMENT_DEFINITION(Effect);

#ifdef _DEBUG
	std::string m_name;
#endif
private:
    std::map<size_t, Technique> m_techniques;
};
