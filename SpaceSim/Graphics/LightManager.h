#pragma once
#include <map>
#include <string>

#include "Core/MessageSystem/Messages.h"

#include "light.h"

class LightManager
{
public:
    LightManager() {}
    ~LightManager() {}

    void addLight(const std::string& name, const Light& light);
    const Light* getLight(const std::string& name) const;
    Light* getLight(const std::string& name);
    
    const std::vector<const Light*> getLights() const
    {
        std::vector<const Light*> lights;
        for (LightMap::const_iterator it = m_lights.begin(); it != m_lights.end(); ++it)
        {
            lights.push_back(&(it->second));
        }
        return lights;
    }

    void dispatchMessage(const MessageSystem::Message& message);
private:
    typedef std::map< size_t, Light> LightMap;
    typedef std::pair< size_t, Light> LightPair;

    LightMap m_lights;
};

