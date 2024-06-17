#pragma once
#include "GameObject.h"

class InfinitySphere : public GameObject
{
public:
    InfinitySphere(Resource* resource) : Super(resource) {}
    virtual ~InfinitySphere() {}

    
    virtual void initialise();
    //virtual void deserialise(const tinyxml2::XMLElement* xmlNode) = 0;
    virtual void update(float elapsedTime, const Input& input);
    HASH_ELEMENT_DEFINITION(InfinitySphere)
protected:
    virtual void DeserialiseInternal(const tinyxml2::XMLElement* node);
    virtual void handleMessage(const MessageSystem::Message& msg);

    void OnDebugImguiInternal() override;
private:
    Vector3 m_scale;
};

