#pragma once
#include "Math/matrix44.h"
#include "Graphics/DeviceManager.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/Resource/RenderResource.h"
#include "Graphics/Shaders.h"
#include <d3d11.h>
#include <D3Dcompiler.h>
#include <string>
#include <map>

class Resource;

namespace tinyxml2
{
    class XMLElement;
}

struct ALIGN16 WVPBufferContent
{
    Matrix44 m_world;
    Matrix44 m_view;
    Matrix44 m_projection;
};

struct ALIGN16 LightConstants
{
    ALIGN16 float m_position[4]; //Element 4 holds inner cone value
    ALIGN16 float m_direction[4]; //Element 4 holds outer cone value
    ALIGN16 float m_diffuse[4];
    ALIGN16 float m_specular[4];
    ALIGN16 float m_attenuationFallOff[4];
    ALIGN16 float m_range[4];//last three elements are empty
};

struct ALIGN16 PerFrameConstants
{
    ALIGN16 LightConstants m_lightConstants[8];
    ALIGN16 float m_cameraPosition[4];
    ALIGN16 WVPBufferContent m_shadowMVP;
};

struct MaterialContent
{
    MaterialContent()
    {
        m_ambient[0] = 0.2f;
        m_ambient[1] = 0.2f;
        m_ambient[2] = 0.2f;
        m_ambient[3] = 1.0f;

        m_diffuse[0] = 0.8f;
        m_diffuse[1] = 0.8f;
        m_diffuse[2] = 0.8f;
        m_diffuse[3] = 1.0f;

        m_specular[0] = 0.0f;
        m_specular[1] = 0.0f;
        m_specular[2] = 0.0f;
        m_specular[3] = 1.0f;

        m_emissive[0] = 0.0f;
        m_emissive[1] = 0.0f;
        m_emissive[2] = 0.0f;
        m_emissive[3] = 1.0f;
        m_shininess = 1000.0f;
    }

    float m_ambient[4];
    float m_diffuse[4];
    float m_specular[4];
    float m_emissive[4];
    ALIGN16 float m_shininess;
};

class Technique
{
public:
    Technique(Resource* resource) : 
        m_vertexShader(0),
        m_hullShader(0),
        m_domainShader(0),
        m_geometryShader(0),
        m_pixelShader(0),
        m_computeShader(0),
        m_resource(resource)
        {}
    ~Technique() {}

    void cleanup() { m_constantBuffers[0]->Release(); m_constantBuffers[1]->Release(); }

    void deserialise(const tinyxml2::XMLElement* element);

    size_t getNameHash() const { return m_nameHash; }

    void setWVPContent(const DeviceManager& deviceManager, const WVPBufferContent& wvpContent) const;
    void setMaterialContent(const DeviceManager& deviceManager, const MaterialContent& materialContent) const;

    size_t getVertexShader() const { return m_vertexShader; }
    size_t getHullShader() const { return m_hullShader; }
    size_t getDomainShader() const { return m_domainShader; }
    size_t getGeometryShader() const { return m_geometryShader; }
    size_t getPixelShader() const { return m_pixelShader; }
    size_t getComputeShader() const { return m_computeShader; }

    //ID3D11VertexShader* getD3DVertexShader() const { return m_vertexShader ? m_vertexShader->getShader() : nullptr; }
    //ID3D11HullShader* getD3DHullShader() const { return m_hullShader ? m_hullShader->getShader() : nullptr; }
    //ID3D11DomainShader* getD3DDomainShader() const { return m_domainShader ? m_domainShader->getShader() : nullptr; }
    //ID3D11GeometryShader* getD3DGeometryShader() const { return m_geometryShader ? m_geometryShader->getShader() : nullptr; }
    //ID3D11PixelShader* getD3DPixelShader() const { return m_pixelShader ? m_pixelShader->getShader() : nullptr; }
    //ID3D11ComputeShader* getD3DComputeShader() const { return m_computeShader ? m_computeShader->getShader() : nullptr; }

    void setupTechnique(bool alsoUseOnGS = false) const
    {
        RenderResourceHelper helper(m_resource);
        ID3D11DeviceContext* deviceContext = helper.getResource().getDeviceManager().getDeviceContext();
        deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffers[0]);
        if (alsoUseOnGS == true)
        {
            deviceContext->GSSetConstantBuffers(0, 1, &m_constantBuffers[0]);
        }
        deviceContext->PSSetConstantBuffers(0, 1, &m_constantBuffers[1]);
    }

    size_t getTechniqueId() const { return m_techniqueId; }

    HASH_ELEMENT_DEFINITION(Technique);
private:
#ifdef _DEBUG
    std::string m_name; //This should be compiled out in release
#endif
    size_t m_techniqueId;
    size_t m_nameHash;
    size_t m_vertexShader;
    size_t m_hullShader;
    size_t m_domainShader;
    size_t m_geometryShader;
    size_t m_pixelShader;
    size_t m_computeShader;
    Resource* m_resource;
public: //TODO remove
    ID3D11Buffer* m_constantBuffers[2]; //Contains the projection, view and world matrices, these should move in to something called constant buffer which can be set by the model
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
    HASH_ELEMENT_DEFINITION(Effect);
private:
    std::map<size_t, Technique> m_techniques;
};
