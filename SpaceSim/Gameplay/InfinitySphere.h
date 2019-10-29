#pragma once
#include "GameObject.h"

class InfinitySphere : public GameObject
{
public:
    InfinitySphere(Resource* resource) : Super(resource) {}
    virtual ~InfinitySphere() {}

    virtual const ShaderInstance deserialise(const tinyxml2::XMLElement* node) ;
    virtual void initialise(const ShaderInstance& shaderInstance);
    //virtual void deserialise(const tinyxml2::XMLElement* xmlNode) = 0;
    virtual void update(RenderInstanceTree& renderInstances, float elapsedTime, const Input& input);
    HASH_ELEMENT_DEFINITION(InfinitySphere)
protected:
    virtual void handleMessage(const MessageSystem::Message& msg);
private:
    Vector3 m_scale;
};

