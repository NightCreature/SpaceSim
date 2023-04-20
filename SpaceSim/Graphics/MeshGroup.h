#pragma once

#include "Graphics/GeometryInstance.h"
#include "Graphics/ShaderInstance.h"
#include "Math/matrix44.h"
#include "Core/Types/Types.h"
#include "Graphics/material.h"

#include "Core/StringOperations/StringHelperFunctions.h"

#include <string>
#include "Graphics/RenderInterface.h"
#include "Core/MessageSystem/GameMessages.h"

#include "..\bin\Shaders\Shaders\BindlessBuffers.h"
#include <unordered_map>

class DeviceManager;
class Resource;
class Bbox;

//Wrapper to hold more then one Geometry instance and material together
class MeshGroup : public RenderInterface
{
public:
    MeshGroup(const Material& material) : m_material(material)
    {
        m_world.identity();
        //m_shaderInstance.AddPsConstantBuffer(sizeof(MaterialContent), deviceManager, "material content buffer for meshgroup");
        //m_shaderInstance.AddVsConstantBuffer(sizeof(WVPBufferContent), deviceManager, "WVP content buffer for meshgroup");
        m_material = material;
        memset(reinterpret_cast<void*>(&m_renderIndices), 0, sizeof(MeshResourceIndices));
    }
    MeshGroup(const MeshGroup& source);
    ~MeshGroup();

    //Create and add a RenderInstance into the tree to be rendered
    void update( Resource* resource, RenderInstanceTree& renderInstance, float elapsedTime, const Matrix44& world, const Matrix44& view, const Matrix44& projection, const std::string& name, const Bbox& box);
    
    void Update(const Matrix44& world, const std::string& name, const Bbox& box);
    void Update(const MessageSystem::RenderInformation::RenderInfo& context);
    
    void UpdateCbs() override {}

    void PopulateCommandlist(Resource* resource, CommandList& list) override;

    void setMaterial(const Material& material) { m_material = material; m_renderInstanceDirty = true; }
    void setWorld(const Matrix44& world) { m_world = world; m_renderInstanceDirty = true; }


    void setDirty() { m_renderInstanceDirty = true; }

    VertexBuffer& GetVB() { return m_vertexBuffer; }
    IndexBuffer& GetIB() { return m_indexBuffer; }
    Material& GetMaterial() { return m_material; }
    size_t CreateConstantBuffer(size_t size, size_t bufferNameHash, const DeviceManager& deviceManager, DescriptorHeap& heap);
    //std::vector<ConstantBuffer>& GetConstantBuffers() { return m_constantBuffers; }
    //const std::vector<ConstantBuffer>& GetConstantBuffers() const { return m_constantBuffers; }
    void SetPrimitiveLayout(size_t layout) { m_primitiveLayout = layout; }

    void Cleanup();

    void SetName(const std::string& name) { m_name = name; }

    void UpdateRenderIndices();
    MeshResourceIndices& GetResourceInices() { return m_renderIndices; }
private:
    std::unordered_map<size_t, ConstantBuffer> m_constantBuffers;

    MeshResourceIndices m_renderIndices;
    Matrix44 m_world;
    Material m_material;
    IndexBuffer m_indexBuffer;
    VertexBuffer m_vertexBuffer;

    std::string m_name;

    size_t m_primitiveLayout;

    //SHould all be done here since we can execute commands here :)
    bool m_renderInstanceDirty;
};

