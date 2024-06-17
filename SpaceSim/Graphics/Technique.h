#pragma once

#include "Core/tinyxml2.h"
#include "Graphics/D3D12/PipelineObject.h"
#include "Graphics/Shaders.h"


#include <array>
#include <string>
#include "Core/StringOperations/HashString.h"

class Resource;

class Technique
{
public:
    Technique(Resource* resource) :
        m_resource(resource)
    {}
    ~Technique() {}

    void cleanup()
    {
        m_pso.Destroy();
    }

    void deserialise(const tinyxml2::XMLElement* element);

    size_t getNameHash() const { return m_nameHash; }

    size_t getTechniqueId() const { return m_techniqueId; }
    const PipelineObject& GetPipelineState() const { return m_pso; }
    PipelineObject& GetPipelineState() { return m_pso; }

    bool IsValid() { return m_pso.IsValid(); }

    void OnDebugImgui() const;

    HASH_ELEMENT_DEFINITION(Technique);

    std::string GetName() const 
    {
#ifdef _DEBUG
        return m_name;
#else
        return m_techniqueId.getString();
#endif
    }
private:

#ifdef _DEBUG
    std::string m_name; //This should be compiled out in release
#endif
    PipelineObject m_pso; //This might bind the actual shaders already
    HashString m_techniqueId;
    HashString m_nameHash;
    Resource* m_resource;
};