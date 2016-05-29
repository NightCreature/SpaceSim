#pragma once

#include "GeometryInstance.h"
#include "ShaderInstance.h"
#include "matrix44.h"
#include "Types.h"

class DeviceManager;
class Resource;

//Wrapper to hold more then one Geometry instance and material together
class MeshGroup
{
public:
    MeshGroup(VertexBuffer* vb, IndexBuffer* ib, const ShaderInstance& shaderInstance) : m_geometryInstance(vb, ib), m_shaderInstance(shaderInstance), m_renderInstance(nullptr)
    {
        m_world.identity();
    }
    MeshGroup(const MeshGroup& source);
    ~MeshGroup();

    //Create and add a RenderInstance into the tree to be rendered
    void update( Resource* resource, RenderInstanceTree& renderInstance, float elapsedTime, const Matrix44& world, const std::string& name);
    
    void setMaterial(const Material& material) { m_shaderInstance.setMaterial( material ); m_renderInstanceDirty = true; }
    void setWorld(const Matrix44& world) { m_world = world; m_renderInstanceDirty = true;  }

    const GeometryInstance& getGeometryInstance() const { return m_geometryInstance; }
    GeometryInstance& getGeometryInstance() { return m_geometryInstance; }
    const ShaderInstance& getShaderInstance() const { return m_shaderInstance; }
    ShaderInstance& getShaderInstance() { return m_shaderInstance; }
    void setShaderInstance(const ShaderInstance& shaderInstance) { m_shaderInstance = shaderInstance; m_renderInstanceDirty = true; }
private:
    Matrix44 m_world;
    GeometryInstance m_geometryInstance;
    ShaderInstance m_shaderInstance;
    RenderInstance* m_renderInstance;
    bool m_renderInstanceDirty;
};

