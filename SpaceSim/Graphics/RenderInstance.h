#pragma once

#include "GeometryInstance.h"
#include "ShaderInstance.h"

#include "xtree"

#ifdef _DEBUG
#include <string>
#endif

//Simple wrapper class for render able data this is what is passed to the renderer
class RenderInstance
{
public:
    RenderInstance(const GeometryInstance* geometryInstance, const ShaderInstance* shaderInstance) : m_geometryInstance(geometryInstance), m_shaderInstance(shaderInstance) {}
    ~RenderInstance(void)
    {
    }

#ifdef _DEBUG
    std::wstring m_name;
#endif

    const ShaderInstance& getShaderInstance() const { return *m_shaderInstance; }
    const GeometryInstance& getGeometryInstance() const { return *m_geometryInstance; }
    const unsigned int getPrimitiveType() const { return m_geometryInstance->getPrimitiveType(); }
private:
    //RenderInstance(const RenderInstance& renderInstance) {}
    //RenderInstance& operator=(const RenderInstance& renderInstance) {}

    const ShaderInstance* m_shaderInstance;
    const GeometryInstance* m_geometryInstance;
};