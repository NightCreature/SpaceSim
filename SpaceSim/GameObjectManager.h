#pragma once

#include <map>
#include <string>
#include <vector>
#include "..\SpaceSim\GameObject.h"
#include "..\SpaceSim\Types.h"

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

    void update(RenderInstanceTree& renderList, float elapsedTime, const Input& input);

    void handleMessage( const Message& message ) const;
private:
    typedef std::pair<unsigned int, GameObject*> ModelPair;
    typedef std::map<unsigned int, GameObject*> GameObjectMap;
    struct DispatchFunctor
    {
        explicit DispatchFunctor(const Message& message) : m_msg(message) {}
        void operator() (const ModelPair& modelPair) { modelPair.second->dispatchMessage(m_msg); }

        Message m_msg;
    };

    GameObjectMap m_gameObjects;
};

