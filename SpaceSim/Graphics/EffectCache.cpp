#include "EffectCache.h"

EffectCache::EffectCache()
{
}


EffectCache::~EffectCache()
{
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void EffectCache::Initialise(Resource* resource)
{
    auto& descriptorHeapManager = RenderResourceHelper(resource).getWriteableResource().getDescriptorHeapManager();
    m_constantBufferHeap = descriptorHeapManager.CreateDescriptorHeap(128, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, true);
}

const Effect* EffectCache::createEffect(Resource* resource, const std::string& resourceFileName)
{
    std::string resourceName = getResourceNameFromFileName(resourceFileName);
    const Effect* returnValue = getEffect(resourceName);
    if (returnValue == nullptr)
    {

        //Load the effect here and deserialise
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(resourceFileName.c_str()) != tinyxml2::XML_NO_ERROR)
        {
            MSG_TRACE_CHANNEL("MAP", "Failed to load %s \nWith error: %d", resourceFileName.c_str(), doc.ErrorID());
            return nullptr;
        }

        const tinyxml2::XMLElement* element;
        element = doc.FirstChildElement(); //skip xml element
        element = element->FirstChildElement();
        auto elementHash = hashString(element->Value());
        if (Effect::m_hash == elementHash)
        {
            Effect effect;
            effect.deserialise(element, resource);
#ifdef _DEBUG
			effect.m_name = resourceName;
#endif
            m_effects.emplace(std::pair<size_t, Effect>(hashString(resourceName), effect));
            returnValue = getEffect(resourceName);
        }
    }

    return returnValue;
}

Effect* EffectCache::getEffect(const std::string& name)
{
    return getEffect(hashString(name));
}

Effect* EffectCache::getEffect(size_t effectHash)
{
    if (m_effects.empty())
    {
        return nullptr;
    }

    std::map<size_t, Effect>::iterator it = m_effects.find(effectHash);
    if (it == m_effects.end())
    {
        return nullptr;
    }

    return &(it->second);
}

void EffectCache::addEffect(const std::string& name, const Effect& effect)
{
    std::map<size_t, Effect>::const_iterator it = m_effects.find(hashString(name));
    if (it == m_effects.end())
    {
        m_effects.emplace(std::pair<size_t, Effect>(hashString(name), effect));
    }
}