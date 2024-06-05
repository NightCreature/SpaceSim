#pragma once

#include "Core/MessageSystem/Messages.h"
#include "Core/MessageSystem/RenderMessages.h"
#include "Gameplay/ECS/System.h"
#include "Gameplay/EntityManager.h"

#include <unordered_map>

namespace ECS
{

class RenderableSystem : public System
{

public:
    void AddEntity(Entity& entity) override;
    void Update() override;
    void HandleMessage(const MessageSystem::Message& message);
private:
    std::unordered_map<size_t, size_t> m_messageData;
};

}