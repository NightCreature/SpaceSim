#pragma once 

#include "material.h"

class DeviceManager;

class ShaderInstance
{
public:
    ShaderInstance()
    {
        m_wvpConstants.m_world.identity();
        m_wvpConstants.m_view.identity();
        m_wvpConstants.m_projection.identity();
    }
    ShaderInstance(const WVPBufferContent& wvp, const Material& material) : m_wvpConstants(wvp), m_material(material) {}
    ~ShaderInstance() {}

    //Add get set operations here
    const Material& getMaterial() const { return m_material; }
    Material& getMaterial() { return m_material; }
    const WVPBufferContent& getWVPConstants() const { return m_wvpConstants; }
    WVPBufferContent& getWVPConstants() { return m_wvpConstants; }
    void setMaterial( const Material& material ) { m_material = material; }
    void setWorld( const Matrix44& world ) { m_wvpConstants.m_world = world; }
private:
    WVPBufferContent m_wvpConstants;
    Material m_material;
};