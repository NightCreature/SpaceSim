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

    Effect* getEffect(const std::string& name);
    Effect* getEffect(size_t effectHash);
    void addEffect(const std::string& name, const Effect& effect);

    DescriptorHeap& GetDescriptorHeap() { return m_constantBufferHeap; }
private:

    std::map<size_t, Effect> m_effects;
    DescriptorHeap m_constantBufferHeap;
};

