#include "EffectCache.h"
#include <Optick.h>

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
    UNUSEDPARAM(resource);
}

const Effect* EffectCache::createEffect(Resource* resource, const std::string& resourceFileName)
{  
    OPTICK_EVENT();
    std::string resourceName = getResourceNameFromFileName(resourceFileName);
    const Effect* returnValue = getEffect(resourceName);
    if (returnValue == nullptr)
    {

        //Load the effect here and deserialise
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(resourceFileName.c_str()) != tinyxml2::XML_NO_ERROR)
        {
            MSG_TRACE_CHANNEL("MAP", "Failed to load %s \nWith error: %d", resourceFileName.c_str(), doc.ErrorID());
            return returnValue;
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
            //This operation needs thread protection
            {
                const std::lock_guard<std::mutex> guard(m_mutex);
                m_effects.emplace(std::pair<size_t, Effect>(hashString(resourceName), effect));
                returnValue = getEffect(resourceName);
            }
        }
    }

    return returnValue;
}

const Effect* EffectCache::getEffect(const std::string& name) const
{
    return getEffect(hashString(name));
}

const Effect* EffectCache::getEffect(size_t effectHash) const
{
    OPTICK_EVENT();
    if (m_effects.empty())
    {
        return nullptr;
    }

    std::map<size_t, Effect>::const_iterator it = m_effects.find(effectHash);
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