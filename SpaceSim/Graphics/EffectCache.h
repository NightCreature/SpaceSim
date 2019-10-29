#pragma once

#include "Effect.h"

#include <map>
#include <string>

class EffectCache
{
public:
    EffectCache();
    ~EffectCache();

    void cleanup() 
    {
        for (auto effect : m_effects)
        {
            effect.second.cleanup();
        }
    }

    const Effect* EffectCache::createEffect(Resource* resource, const std::string& resourceFileName);

    const Effect* getEffect(const std::string& name) const;
    const Effect* getEffect(size_t effectHash) const;
    void addEffect(const std::string& name, const Effect& effect);
private:

    std::map<size_t, Effect> m_effects;
};

