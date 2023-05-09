#pragma once

#include "Core/Types/Types.h"
#include <string>
#include "Math/vector2.h"
#include "Math/vector3.h"
#include "Graphics/color.h"
#include "Gameplay/bbox.h"
#include "Graphics/Effect.h"
#include "Graphics/material.h"
#include "Math/matrix44.h"
#include <limits>  
#include "Graphics/IndexBuffer.h"
#include "Graphics/VertexBuffer.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include <D3D11.h>
#include <Core/tinyxml2.h>
#include <list>
#include "Graphics/DebugBox.h"
#include "Core/MessageSystem//Messages.h"
#include "Core/Types/Types.h"
#include "Core/Types/TypeHelpers.h"
#include "Graphics/Model.h"
#include "Graphics/DebugHelperFunctions.h"
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

    GameObject(Resource* resource) : m_name(""), m_resource(resource), m_nameHash(0), m_active(false), m_worldhaschanged(false), m_initialisationDone(false)
    { m_world.identity(); }
    virtual ~GameObject(void) 
    {
    }
    virtual void deserialise( const tinyxml2::XMLElement* node ) = 0;
    virtual void initialise() 
    {
    }
    //virtual void deserialise(const tinyxml2::XMLElement* xmlNode) = 0;
    virtual void update(float elapsedTime, const Input& input) 
    {
        UNUSEDPARAM(elapsedTime);
        UNUSEDPARAM(input);

        //This should genrate a render message to the render thread so it can be rendered
    }

    void dispatchMessage( const MessageSystem::Message& msg)
    {
        handleMessage(msg);
    }

    bool getActive() const {return m_active;}
    void setActive(bool active) {m_active = active;}

    const std::string& getName() const { if (m_nameHash == 0) { hashString(m_name); } return m_name; }
    const Resource& getResource() const { return *m_resource; }

    bool collision(const Bbox& bbox, const Vector3& dir)
    {
        UNUSEDPARAM(bbox);
        UNUSEDPARAM(dir);
        //if(0.0f < intersectMovingBboxes(m_drawableObject->getBoundingBox(), bbox, dir))
        //    return false;
        return true;
    }
    float collision(const Vector3& position, const Vector3& dir, Vector3& normal) 
    {
        UNUSEDPARAM(position);
        UNUSEDPARAM(dir);
        UNUSEDPARAM(normal);
        return true;
        //return m_drawableObject->getBoundingBox().rayintersect(position, dir, normal);
    }

    //void setMaterial(const Material& mat) { m_drawableObject ? m_drawableObject->getRenderInstance()->getShaderInstance().setMaterial(getResource().getDeviceManager(), mat) : MSG_TRACE_CHANNEL("MODEL", "Trying to set the material on shape without a drawable object" ) assert(m_drawableObject); }
    //const Material* getMaterial() const { return m_drawableObject ? &m_drawableObject->getRenderInstance()->getShaderInstance().getMaterial() : 0; }
    const Bbox& getBbox() const {return Bbox();}
    Matrix44 getWorld() {return m_world;}
    void setWorld(const Matrix44& m) {m_worldhaschanged = true; m_world = m;}

    bool IsInitialising() const { return !m_initialisationDone; }

    void SetRenderHandle(size_t renderId) { m_renderHandle = renderId; }

    virtual void handleMessage(const MessageSystem::Message& msg) = 0;
protected:

    Matrix44 m_world;
    std::string m_name;
    Resource* m_resource;
    size_t m_renderHandle;
    size_t m_nameHash;
    bool m_active;
    bool m_worldhaschanged;
    bool m_initialisationDone;
};
