#pragma once

#include "..\SpaceSim\Types.h"
#include <string>
#include "vector2.h"
#include "vector3.h"
#include "color.h"
#include "bbox.h"
#include "..\SpaceSim\Effect.h"
#include "material.h"
#include "matrix44.h"
#include <limits>  
#include "..\SpaceSim\IndexBuffer.h"
#include "..\SpaceSim\VertexBuffer.h"
#include "..\SpaceSim\StringHelperFunctions.h"
#include <D3D11.h>
#include <..\SpaceSim\tinyxml2.h>
#include <list>
#include "..\SpaceSim\DebugBox.h"
#include "..\SpaceSim\Messages.h"
#include "..\SpaceSim\Types.h"
#include "..\SpaceSim\TypeHelpers.h"
#include "Model.h"

#include <assert.h>

class Input;
class Message;
class Resource;
class GameResource;

namespace tinyxml2
{
    class XMLNode;
}


class GameObject
{
public:
    typedef GameObject Super;

    GameObject(Resource* resource) : m_name(""), m_resource(resource), m_drawableObject(nullptr), m_nameHash(0), m_active(false), m_worldhaschanged(false)
#if defined(SHOW_BOUNDING_BOXES)
        , m_debugBox(nullptr)
#endif
    { m_world.identity(); }
    virtual ~GameObject(void) 
    { 
        if (m_drawableObject != nullptr)
        {
            delete m_drawableObject;
            m_drawableObject = nullptr;
        }

#if defined(SHOW_BOUNDING_BOXES)
        if (m_debugBox != nullptr)
        {
            delete m_debugBox;
            m_debugBox = nullptr;
        }
#endif
    }
    virtual const ShaderInstance deserialise( const tinyxml2::XMLElement* node ) = 0;
    virtual void initialise(const ShaderInstance& shaderInstance) 
    {
        UNUSEDPARAM(shaderInstance); 

		m_drawableObject->getOriginalBoundingBox() = m_drawableObject->getBoundingBox();
        m_drawableObject->getBoundingBox().transformAccordingToMatrix(m_world);

#if defined(SHOW_BOUNDING_BOXES)
        m_debugBox =  new DebugGraphics::DebugBox(m_resource, m_drawableObject->getBoundingBox().getMin(), m_drawableObject->getBoundingBox().getMax());
        m_debugBox->initialise(ShaderInstance());
#endif
    }
    //virtual void deserialise(const tinyxml2::XMLElement* xmlNode) = 0;
    virtual void update(RenderInstanceTree& renderInstances,  float elapsedTime, const Input& input) 
    {
        UNUSEDPARAM(input);
		m_drawableObject->getBoundingBox() = m_drawableObject->getOriginalBoundingBox();
        m_drawableObject->getBoundingBox().transformAccordingToMatrix(m_world);
        m_drawableObject->update(m_resource, renderInstances, elapsedTime, m_world, m_name);

#if defined(SHOW_BOUNDING_BOXES)
        static Matrix44 temp;
        temp.identity();
        if (m_debugBox != nullptr)
        {
            m_debugBox->update(renderInstances, elapsedTime, temp, "Bounding box" + m_name);
        }
#endif
    }

    void dispatchMessage( const Message& msg)
    {
        if (m_name == msg.getTarget())
        {
            handleMessage(msg);
        }
    }

    bool getActive() const {return m_active;}
    void setActive(bool active) {m_active = active;}

    const std::string& getName() const { return m_name; }
    const Resource& getResource() const { return *m_resource; }
    const GameResource& getGameResource() const { return *(GameResource*)m_resource; }
    GameResource& getWriteableGameResource() const { return *(GameResource*)m_resource; } //This should return a reference

    bool collision(const Bbox& bbox, const Vector3& dir)
    {
        if(0.0f < intersectMovingBboxes(m_drawableObject->getBoundingBox(), bbox, dir))
            return false;
        return true;
    }
    float collision(const Vector3& position, const Vector3& dir, Vector3& normal) 
    {
        return m_drawableObject->getBoundingBox().rayintersect(position, dir, normal);
    }

    //void setMaterial(const Material& mat) { m_drawableObject ? m_drawableObject->getRenderInstance()->getShaderInstance().setMaterial(getGameResource().getDeviceManager(), mat) : MSG_TRACE_CHANNEL("MODEL", "Trying to set the material on shape without a drawable object" ) assert(m_drawableObject); }
    //const Material* getMaterial() const { return m_drawableObject ? &m_drawableObject->getRenderInstance()->getShaderInstance().getMaterial() : 0; }
    const Bbox& getBbox() const {return m_drawableObject->getBoundingBox();}
    Matrix44 getWorld() {return m_world;}
    void setWorld(const Matrix44& m) {m_worldhaschanged = true; m_world = m;}
protected:
    virtual void handleMessage( const Message& msg) = 0;

    Matrix44 m_world;
    std::string m_name;
    Resource* m_resource;
    Model* m_drawableObject;
    unsigned int m_nameHash;
    bool m_active;
    bool m_worldhaschanged;

#if defined(SHOW_BOUNDING_BOXES)
    DebugGraphics::DebugBox* m_debugBox;
#endif
};
