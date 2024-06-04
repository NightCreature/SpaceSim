#pragma once

#include "Gameplay/ECS/Component.h"

#include <unordered_map>
namespace ECS
{

class EntityTagManager
{
public:
    EntityTagManager() {}


    template<class CreateComponentFunction>
    void RegisterComponentTag(const ComponentTag& tag, CreateComponentFunction createFunc)
    {
        auto iterator = std::find_if(cbegin(m_componentCreationFuncs), cend(m_componentCreationFuncs), [tag](const auto& tagFuncPair) { return tag == tagFuncPair.first; });
        if (iterator == cend(m_componentCreationFuncs))
        {
            m_componentCreationFuncs.emplace_back(make_pair(tag, createFunc));
        }
    }

    Component* CreateForTag(const ComponentTag& tag);

private:
    std::vector<std::pair<ComponentTag, ComponentDataCreationFp>> m_componentCreationFuncs;
};

}

