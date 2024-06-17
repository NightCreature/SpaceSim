#include "EffectCache.h"

#include "Core/Profiler/ProfilerMacros.h"
#include "Core/Thread/JobSystem.h"
#include "Logging/LoggingMacros.h"
#include "Graphics/modelmanager.h"
#include "imgui.h"

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

void EffectCache::CreateEffect(Resource* resource, const std::string& resourceFileName)
{  
    PROFILE_FUNCTION();

    auto effectCreatedCallback = [resource](Effect& effect) 
        {
            MSG_TRACE_CHANNEL_FMT("EffectCache", "Create PSOs for effect {}", effect.m_name);
            //From here we can create the PSO state.
            effect.createPSOs();

            ModelManager& modelManager = RenderResourceHelper(resource).getWriteableResource().getModelManager();
            modelManager.UpdateSortKeysAndSortModels();


            return true;
        };

    //Rewrite this so its can be done on a job, which means we create the effect entry in the map and pass that along to the job
    std::string resourceName = getResourceNameFromFileName(resourceFileName);
    bool hasEffect = HasEffect(resourceName);
    MSG_TRACE_CHANNEL("EffectCache", "Creating effect %s, effect is %s present", resourceName.c_str(), hasEffect ? "" : "not");
    if (!hasEffect) //If the effect is not in the cache
    {
        Effect& effectToBeCreated = m_effects[HashString(resourceName)]; //Make the effect entry in the map
        LoadEffectJob* job = new LoadEffectJob(resourceFileName, effectToBeCreated, effectCreatedCallback); //Create the job
        static_cast<RenderResource*>(resource)->getJobQueue().AddJob(job); //Add the job to the queue
    }
}

bool EffectCache::HasEffect(const std::string& name) const
{
    return getEffect(HashString(name)) != nullptr;
}

void EffectCache::OnDebugImgui() const
{
    ImGui::Begin("Effects");

    for (const auto& effect : m_effects)
    {
        std::string nodeName;
#ifdef DEBUG
        nodeName = effect.first.getString();
#else
        nodeName = fmt::format("Effect: {}", effect.first.getHash());
#endif // DEBUG

        if (ImGui::TreeNode(nodeName.c_str()))
        {
            effect.second.OnDebugImgui();
            ImGui::TreePop();
        }
    }

    ImGui::End();
}

const Effect* EffectCache::getEffect(const std::string& name) const
{
    return getEffect(HashString(name));
}

const Effect* EffectCache::getEffect(const HashString& effectHash) const
{
    PROFILE_FUNCTION();
    if (m_effects.empty())
    {
        return nullptr;
    }

    std::map<HashString, Effect>::const_iterator it = m_effects.find(effectHash);
    if (it == m_effects.end())
    {
        return nullptr;
    }

    return &(it->second);
}

void EffectCache::addEffect(const std::string& name, const Effect& effect)
{
    std::map<HashString, Effect>::const_iterator it = m_effects.find(HashString(name));
    if (it == m_effects.end())
    {
        m_effects.emplace(std::pair<HashString, Effect>(HashString(name), effect));
    }
}

bool LoadEffectJob::Execute(ThreadContext* context)
{
    PROFILE_FUNCTION();    

    //Load the effect here and deserialise
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(m_filePath.string().c_str()) != tinyxml2::XML_NO_ERROR)
    {
        MSG_TRACE_CHANNEL("CreateEffectJob", "Failed to load %s \nWith error: %d", m_filePath.string().c_str(), doc.ErrorID());
        return false;
    }

    MSG_TRACE_CHANNEL_FMT("CreateEffectJob", "deserialising effect {}", m_filePath.string());

    const tinyxml2::XMLElement* element;
    element = doc.FirstChildElement(); //skip xml element
    element = element->FirstChildElement();
    auto elementHash = HashString(element->Value());
    if (Effect::m_hash == elementHash)
    {
        m_effect.deserialise(element, context->m_renderResource);
#ifdef _DEBUG
        m_effect.m_name = getResourceNameFromFileName(m_filePath.string());
#endif
    }

    return true;
}
