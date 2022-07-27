#pragma once

#include "Effect.h"

#include <map>
#include <string>
#include "D3D12/DescriptorHeapManager.h"

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

    void Initialise(Resource* resource);

    const Effect* createEffect(Resource* resource, const std::string& resourceFileName);

    const Effect* getEffect(const std::string& name) const;
    const Effect* getEffect(size_t effectHash) const;
    void addEffect(const std::string& name, const Effect& effect);
private:

    std::map<size_t, Effect> m_effects;
};

