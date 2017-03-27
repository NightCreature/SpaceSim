#pragma once

#include "GeometryInstance.h"
#include "ShaderInstance.h"
#include "Gameplay/bbox.h"
#include "xtree"

#ifdef _DEBUG
#include <string>
#endif

//Simple wrapper class for render able data this is what is passed to the renderer
class RenderInstance
{
public:
    RenderInstance(GeometryInstance* geometryInstance, const ShaderInstance* shaderInstance) :
        m_geometryInstance(geometryInstance),
        m_shaderInstance(*shaderInstance)
    {}
    RenderInstance(const Bbox& boundingBox, GeometryInstance* geometryInstance, const ShaderInstance* shaderInstance) : 
        m_boundingBox(boundingBox),
        m_geometryInstance(geometryInstance),
        m_shaderInstance(*shaderInstance) 
    {}
    ~RenderInstance(void)
    {
    }

#ifdef _DEBUG
    std::wstring m_name;
#endif

    const ShaderInstance& getShaderInstance() const { return m_shaderInstance; }
    const GeometryInstance& getGeometryInstance() const { return *m_geometryInstance; }
    const unsigned int getPrimitiveType() const { return m_geometryInstance->getPrimitiveType(); }

    void setShaderInstance(ShaderInstance* instance) { m_shaderInstance = *instance; }
    void setGeometryInstance(GeometryInstance* instance) { m_geometryInstance = instance; }

    void setBoundingBox(const Bbox& boundingBox) { m_boundingBox = boundingBox; }
    const Bbox& getBoundingBox() const { return m_boundingBox; }
private:
    //RenderInstance(const RenderInstance& renderInstance) {}
    //RenderInstance& operator=(const RenderInstance& renderInstance) {}

    Bbox m_boundingBox;
    ShaderInstance m_shaderInstance;
    const GeometryInstance* m_geometryInstance;
};