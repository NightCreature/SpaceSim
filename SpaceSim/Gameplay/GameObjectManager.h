#pragma once

#include <map>
#include <string>
#include <vector>
#include "Gameplay/GameObject.h"
#include "Core/Types/Types.h"

class GameObjectManager
{
public:
    GameObjectManager(void);
    ~GameObjectManager(void);

    void cleanup();

    void addGameObject(GameObject* model);
    void removeGameObject(GameObject* model);
    const GameObject* getGameObject(const std::string& objectName) const;
    const std::vector<GameObject*> getGameObjectsThatContain(const std::string& partOfObjectName) const;
    const std::vector<GameObject*> getGameObjectsThatDontContain(const std::string& partOfObjectName) const;

    void update(float elapsedTime, const Input& input);

    void handleMessage( const MessageSystem::Message& message );
    void addMessage(const Message& message);

    void OnDebugImgui();
private:
    typedef std::pair<size_t, GameObject*> ModelPair;
    typedef std::map<size_t, GameObject*> GameObjectMap;
    //struct DispatchFunctor
    //{
    //    explicit DispatchFunctor(const Message& message) : m_msg(message) {}
    //    void operator() (const ModelPair& modelPair) { modelPair.second->dispatchMessage(m_msg); }

    //    Message m_msg;
    //};

    GameObjectMap m_gameObjects;
};

