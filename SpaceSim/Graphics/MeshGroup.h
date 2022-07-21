#pragma once

#include "Graphics/GeometryInstance.h"
#include "Graphics/ShaderInstance.h"
#include "Math/matrix44.h"
#include "Core/Types/Types.h"
#include "Graphics/material.h"

#include "Core/StringOperations/StringHelperFunctions.h"

class DeviceManager;
class Resource;
class Bbox;

//Wrapper to hold more then one Geometry instance and material together
class MeshGroup
{
public:
    MeshGroup(VertexBuffer* vb, IndexBuffer* ib, const Material& material, DeviceManager& deviceManager) : m_geometryInstance(vb, ib), m_material(material), m_renderInstance(nullptr)
    {
        m_world.identity();
        m_shaderInstance.setMaterial(m_material);
        //m_shaderInstance.AddPsConstantBuffer(sizeof(MaterialContent), deviceManager, "material content buffer for meshgroup");
        //m_shaderInstance.AddVsConstantBuffer(sizeof(WVPBufferContent), deviceManager, "WVP content buffer for meshgroup");
        m_material = material;
        UNUSEDPARAM(deviceManager);
    }
    MeshGroup(const MeshGroup& source);
    ~MeshGroup();

    //Create and add a RenderInstance into the tree to be rendered
    void update( Resource* resource, RenderInstanceTree& renderInstance, float elapsedTime, const Matrix44& world, const Matrix44& view, const Matrix44& projection, const std::string& name, const Bbox& box);
    
    void Update(Resource* resource, CommandList& list, float elapsedTime, const Matrix44& world, const std::string& name, const Bbox& box);

    void setMaterial(const Material& material) { m_material = material; m_renderInstanceDirty = true; }
    void setWorld(const Matrix44& world) { m_world = world; m_renderInstanceDirty = true; }

    const GeometryInstance& getGeometryInstance() const { return m_geometryInstance; }
    GeometryInstance& getGeometryInstance() { return m_geometryInstance; }
    const ShaderInstance& getShaderInstance() const { return m_shaderInstance; }
    ShaderInstance& getShaderInstance() { return m_shaderInstance; }
    void setShaderInstance(const ShaderInstance& shaderInstance) { m_shaderInstance = shaderInstance; m_renderInstanceDirty = true; }

    void setDirty() { m_renderInstanceDirty = true; }
private:
    Matrix44 m_world;
    Material m_material;
    GeometryInstance m_geometryInstance; //This goes
    ShaderInstance m_shaderInstance; //this goes
    RenderInstance* m_renderInstance; //this goes
    //SHould all be done here since we can execute commands here :)
    bool m_renderInstanceDirty;

public:
    //shouldnt be here just want to be able to draw something
    static Matrix44 m_projection;
    static Matrix44 m_view;
};

