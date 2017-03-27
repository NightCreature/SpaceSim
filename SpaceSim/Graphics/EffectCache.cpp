#include "EffectCache.h"

EffectCache::EffectCache()
{
}


EffectCache::~EffectCache()
{
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
        unsigned int elementHash = hashString(element->Value());
        if (Effect::m_hash == elementHash)
        {
            Effect effect;
            effect.deserialise(element, resource);
            m_effects.emplace(std::pair<unsigned int, Effect>(hashString(resourceName), effect));
            returnValue = getEffect(resourceName);
        }
    }

    return returnValue;
}

const Effect* EffectCache::getEffect(const std::string& name) const
{
    if (m_effects.empty())
    {
        return nullptr;
    }

    std::map<unsigned int, Effect>::const_iterator it = m_effects.find(hashString(name));
    if (it == m_effects.end())
    {
        return nullptr;
    }

    return &(it->second);
}

void EffectCache::addEffect(const std::string& name, const Effect& effect)
{
    std::map<unsigned int, Effect>::const_iterator it = m_effects.find(hashString(name));
    if (it == m_effects.end())
    {
        m_effects.emplace(std::pair<unsigned int, Effect>(hashString(name), effect));
    }
}