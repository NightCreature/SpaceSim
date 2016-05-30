#include "LightManager.h"


//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void LightManager::addLight( const std::string& name, const Light& light )
{
    unsigned int hashedName = hashString(name);
    LightMap::const_iterator it = m_lights.find(hashedName);
    if (it == m_lights.end())
    {
        m_lights.insert( LightPair(hashedName, light) );
    }
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const Light* LightManager::getLight( const std::string& name ) const
{
    unsigned int hashedName = hashString(name);
    LightMap::const_iterator it = m_lights.find(hashedName);
    if (it != m_lights.end())
    {
        return &(it->second);
    }

    return nullptr;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
Light* LightManager::getLight( const std::string& name )
{
    unsigned int hashedName = hashString(name);
    LightMap::iterator it = m_lights.find(hashedName);
    if (it != m_lights.end())
    {
        return &(it->second);
    }

    return nullptr;
}
