#include "Gameplay/GameObjectManager.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "UI/Messages.h"

#include "Core/Profiler/ProfilerMacros.h"

#include "Core/MessageSystem/RenderMessages.h"

GameObjectManager::GameObjectManager(void)
{
}


GameObjectManager::~GameObjectManager(void)
{
    //Should delete all game objects as this is the owner
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void GameObjectManager::addGameObject( GameObject* model )
{
    m_gameObjects.insert( ModelPair(hashString(model->getName()), model) );
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void GameObjectManager::removeGameObject( GameObject* model )
{
    GameObjectMap::iterator gameObjectIt = m_gameObjects.find( hashString(model->getName()) );
    if (gameObjectIt != m_gameObjects.end())
    {
        m_gameObjects.erase(gameObjectIt);
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const GameObject* GameObjectManager::getGameObject( const std::string& objectName ) const
{
    GameObjectMap::const_iterator gameObjectIt = m_gameObjects.find(hashString(objectName));
    if (m_gameObjects.end() != gameObjectIt)
    {
        return gameObjectIt->second;
    }

    return nullptr;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const std::vector<GameObject*> GameObjectManager::getGameObjectsThatContain( const std::string& partOfObjectName ) const
{
    std::vector<GameObject*> returnValue;
    for(GameObjectMap::const_iterator gameObjectIt = m_gameObjects.begin(); gameObjectIt != m_gameObjects.end(); ++gameObjectIt )
    {
        if ( gameObjectIt->second->getName().find(partOfObjectName) != std::string::npos)
        {
            returnValue.push_back(gameObjectIt->second);
        }
    }

    return returnValue;
}

const std::vector<GameObject*> GameObjectManager::getGameObjectsThatDontContain(const std::string& partOfObjectName) const
{
    std::vector<GameObject*> returnValue;
    for(GameObjectMap::const_iterator gameObjectIt = m_gameObjects.begin(); gameObjectIt != m_gameObjects.end(); ++gameObjectIt )
    {
        if ( gameObjectIt->second->getName().find(partOfObjectName) == std::string::npos)
        {
            returnValue.push_back(gameObjectIt->second);
        }
    }

    return returnValue;
}

void GameObjectManager::update(float elapsedTime, const Input& input)
{
    PROFILE_EVENT("GameObjectManagerUpdate", Red);
    OPTICK_EVENT();
    for (auto gameObject : m_gameObjects)
    {
        if (gameObject.second != nullptr && !gameObject.second->IsInitialising())
        {
            gameObject.second->update(elapsedTime, input);
        }
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void GameObjectManager::handleMessage( const MessageSystem::Message& message )
{
    OPTICK_EVENT();
    UNUSEDPARAM(message);
    //std::for_each(m_gameObjects.begin(), m_gameObjects.end(), DispatchFunctor(message));
    if (MESSAGE_ID(CreatedRenderResourceMessage) == message.getMessageId())
    { 
        auto msg = static_cast<const MessageSystem::CreatedRenderResourceMessage&>(message);
        auto data = msg.GetData();
        GameObjectMap::iterator it = m_gameObjects.find(data->m_gameObjectId);
        if (it != m_gameObjects.end())
        {
            it->second->handleMessage(message);
        }
    }
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void GameObjectManager::addMessage(const Message& message)
{
    UNUSEDPARAM(message);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void GameObjectManager::cleanup()
{
    for (GameObjectMap::iterator it = m_gameObjects.begin(); it != m_gameObjects.end(); ++it)
    {
        GameObject* gameObject = it->second;
        if (gameObject)
        {
            delete gameObject;
        }
        it->second = 0;
    }
}
