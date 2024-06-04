#pragma once

#include "Core/Resource/RenderResource.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/D3D12/ConstantBuffer.h"
#include "Graphics/D3D12/DeviceManagerD3D12.h"
#include "Graphics/D3D12/PipelineObject.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/Shaders.h"
#include "Graphics/Technique.h"
#include "Math/matrix44.h"

#include <D3Dcompiler.h>
#include <d3d11.h>
#include <map>
#include <string>

class Resource;

namespace tinyxml2
{
    class XMLElement;
}



class Effect
{
public:
    Effect() {}
    ~Effect(void) {}

    void cleanup() { for (auto technique : m_techniques) { technique.second.cleanup(); } }

    void deserialise(const tinyxml2::XMLElement* node, Resource* resource);
    void setupEffect() const;
    void createPSOs();

    bool IsValid();

    const Technique* getTechnique(const std::string& techniqueName) const;
    const Technique* getTechnique(const size_t techniqueName) const;

    Technique* getTechnique(const std::string& techniqueName);
    Technique* getTechnique(const size_t techniqueName);
    HASH_ELEMENT_DEFINITION(Effect);

#ifdef _DEBUG
	std::string m_name;
#endif
private:
    std::map<size_t, Technique> m_techniques;
    Resource* m_resource = nullptr;
};
