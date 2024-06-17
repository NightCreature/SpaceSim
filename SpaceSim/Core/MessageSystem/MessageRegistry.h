#pragma once

#include "Core/MessageSystem/MessageType.h"

#include <unordered_map>
#include <unordered_set>


namespace MessageSystem
{

    struct MessageTypeRegistry
    {
        using TypesById = std::unordered_map<std::string_view, const MessageType*>;
        
        using IdToCreationFp = std::unordered_map<std::string_view, CreateMessageFP>;

        TypesById m_messageTypes;
        
        IdToCreationFp m_idToCreationFp;
    };

    static ComponentRegistry theCompoenentRegister;
    inline ComponentRegistry& GetComponentRegistry() { return theCompoenentRegister; }

    void RegisterComponentType(const ComponentType* type);

    template<class Component>
    ComponentTag RegisterComponent(const char* componentName, ComponentTag componentTag = CreateUniqueTag())
    {
        static DefaultComponentType<Component> componentType(componentName, componentTag);

        RegisterComponentType(&componentType);

        return componentTag;
    }

    inline const ComponentTag& GetComponentTagForComponent(const char* componentName)
    {
        auto& registery = GetComponentRegistry();
        return registery.m_idToTag[componentName];
    }

    inline const ComponentType* GetComponentTypeForComponent(const char* componentName)
    {
        auto& registery = GetComponentRegistry();
        if (registery.m_componentTypes.contains(componentName))
        {
            return registery.m_componentTypes[componentName];
        }

        return nullptr;
    }

    inline const ComponentType* GetComponentTypeForComponent(const std::string_view& componentName)
    {
        auto& registery = GetComponentRegistry();
        if (registery.m_componentTypes.contains(componentName))
        {
            return registery.m_componentTypes[componentName];
        }

        return nullptr;
    }

    inline Component* CreateComponent(const char* componentName)
    {
        auto& registery = GetComponentRegistry();
        return registery.m_idToCreationFp[componentName]();
    }


    inline Component* CreateComponent(const std::string_view& componentName)
    {
        auto& registery = GetComponentRegistry();
        return registery.m_idToCreationFp[componentName]();
    }

}
