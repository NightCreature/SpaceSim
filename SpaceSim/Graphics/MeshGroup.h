#pragma once

#include "Graphics/GeometryInstance.h"
#include "Graphics/ShaderInstance.h"
#include "Math/matrix44.h"
#include "Core/Types/Types.h"
#include "Graphics/material.h"

#include "Core/StringOperations/StringHelperFunctions.h"

#include <string>

class DeviceManager;
class Resource;
class Bbox;

//Wrapper to hold more then one Geometry instance and material together
class MeshGroup
{
public:
    MeshGroup(const Material& material) : m_material(material)
    {
        m_world.identity();
        //m_shaderInstance.AddPsConstantBuffer(sizeof(MaterialContent), deviceManager, "material content buffer for meshgroup");
        //m_shaderInstance.AddVsConstantBuffer(sizeof(WVPBufferContent), deviceManager, "WVP content buffer for meshgroup");
        m_material = material;
    }
    MeshGroup(const MeshGroup& source);
    ~MeshGroup();

    //Create and add a RenderInstance into the tree to be rendered
    void update( Resource* resource, RenderInstanceTree& renderInstance, float elapsedTime, const Matrix44& world, const Matrix44& view, const Matrix44& projection, const std::string& name, const Bbox& box);
    
    void Update(Resource* resource, CommandList& list, float elapsedTime, const Matrix44& world, const std::string& name, const Bbox& box);

    void PopulateCommandlist(Resource* resource, CommandList& list);

    void setMaterial(const Material& material) { m_material = material; m_renderInstanceDirty = true; }
    void setWorld(const Matrix44& world) { m_world = world; m_renderInstanceDirty = true; }


    void setDirty() { m_renderInstanceDirty = true; }

    VertexBuffer& GetVB() { return m_vertexBuffer; }
    IndexBuffer& GetIB() { return m_indexBuffer; }
    Material& GetMaterial() { return m_material; }
    void CreateConstantBuffer(size_t size, size_t rootParamIndex, const DeviceManager& deviceManager, DescriptorHeap& heap);
    //std::vector<ConstantBuffer>& GetConstantBuffers() { return m_constantBuffers; }
    //const std::vector<ConstantBuffer>& GetConstantBuffers() const { return m_constantBuffers; }
    void SetPrimitiveLayout(size_t layout) { m_primitiveLayout = layout; }

    void Cleanup();

    void SetName(const std::string& name) { m_name = name; }
private:
    std::vector<std::pair<size_t, ConstantBuffer>> m_constantBuffers;
    Matrix44 m_world;
    Material m_material;
    IndexBuffer m_indexBuffer;
    VertexBuffer m_vertexBuffer;

    std::string m_name;

    size_t m_primitiveLayout;

    //SHould all be done here since we can execute commands here :)
    bool m_renderInstanceDirty;

public:
    //shouldnt be here just want to be able to draw something
    static Matrix44 m_projection;
    static Matrix44 m_view;
};

