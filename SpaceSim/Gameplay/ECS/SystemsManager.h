#pragma once

#include <unordered_map>

namespace ECS
{

class System;

enum UpdatePolicy : size_t
{
    PrePhysics = 1,
    PostPhysics = 2,
    Update = 4
};

class SystemsManager
{
public:
    void RegisterSystem(System* system, const UpdatePolicy& policy);

    void PrePhysicsUpdate();
    void Update();
    void PostPhysicsUpdate();

private:
    std::unordered_map<UpdatePolicy, std::vector<System*>> m_systems; //This might want to store callbacks to the system update calls

};

}