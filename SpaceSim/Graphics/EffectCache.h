#pragma once

#include "Effect.h"

#include <map>
#include <string>
#include "D3D12/DescriptorHeapManager.h"
#include <mutex>
#include "Core/Thread/Job.h"
#include <functional>

class LoadEffectJob : public Job
{
public:

    LoadEffectJob(const std::filesystem::path& filePath, Effect& effect, auto callback) : m_filePath(filePath), m_effect(effect), m_callback(callback) {}
    bool Execute(ThreadContext* context) override;

    void Finish(ThreadContext* context) override
    {
        UNUSEDPARAM(context);
        m_callback(m_effect);
    }
private:
    std::filesystem::path m_filePath;
    Effect& m_effect;
    std::function<void(Effect& effect)> m_callback;
};

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

    void CreateEffect(Resource* resource, const std::string& resourceFileName);

    const Effect* getEffect(const std::string& name) const;
    const Effect* getEffect(size_t effectHash) const;
    void addEffect(const std::string& name, const Effect& effect);
    bool HasEffect(const std::string& name) const;
private:

    std::map<size_t, Effect> m_effects;

    std::mutex m_mutex;
};

