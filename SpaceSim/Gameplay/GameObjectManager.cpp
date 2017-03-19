#include "Gameplay/GameObjectManager.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "UI/Messages.h"

#include "Brofiler.h"

#include "Core/MessageSystem/RenderMessages.h"

GameObjectManager::GameObjectManager(void)
{
}


GameObjectManager::~GameObjectManager(void)
{
    //Should delete all game objects as this is the owner
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void GameObjectManager::addGameObject( GameObject* model )
{
    m_gameObjects.insert( ModelPair(hashString(model->getName()), model) );
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void GameObjectManager::removeGameObject( GameObject* model )
{
    GameObjectMap::iterator gameObjectIt = m_gameObjects.find( hashString(model->getName()) );
    if (gameObjectIt != m_gameObjects.end())
    {
        m_gameObjects.erase(gameObjectIt);
    }
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const GameObject* GameObjectManager::getGameObject( const std::string& objectName ) const
{
    GameObjectMap::const_iterator gameObjectIt = m_gameObjects.find(hashString(objectName));
    if (m_gameObjects.end() != gameObjectIt)
    {
        return gameObjectIt->second;
    }

    return nullptr;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
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

void GameObjectManager::update(RenderInstanceTree& renderList, float elapsedTime, const Input& input)
{
    BROFILER_CATEGORY("GameObjectManagerUpdate", Profiler::Color::Red);
    for (auto gameObject : m_gameObjects)
    {
        gameObject.second->update(renderList, elapsedTime, input);
    }
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void GameObjectManager::handleMessage( const MessageSystem::Message& message )
{
    UNUSEDPARAM(message);
    //std::for_each(m_gameObjects.begin(), m_gameObjects.end(), DispatchFunctor(message));
    if (MESSAGE_ID(CreatedRenderResourceMessage) == message.getMessageId())
    { 
        auto msg = static_cast<const MessageSystem::CreatedRenderResourceMessage&>(message);
        auto data = msg.GetData();
        if (m_gameObjects[static_cast<unsigned int>(data->m_gameObjectId)] != nullptr)
        {
            m_gameObjects[static_cast<unsigned int>(data->m_gameObjectId)]->SetRenderHandle(data->m_renderResourceHandle);
        }
    }
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void GameObjectManager::addMessage(const Message& message)
{
    UNUSEDPARAM(message);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
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
