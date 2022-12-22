#pragma once


//
//void MC_RegisterNodeType(MC_NodeType* aNodeType)
//{
//    MC_ASSERT(MT_ThreadingTools::IsMainThread);
//
//#ifdef MC_ENABLE_CONSOLE
//    if (!locConsoleRegistered)
//    {
//        if (MC_Console* console = MC_Console::GetInstance())
//        {
//            console->AddFunc("Nodes.PrintSizes", PrintNodeSizes);
//            locConsoleRegistered = true;
//        }
//    }
//#endif
//
//    MC_NodeTypeRegistry& ntr = GetNodeTypeRegistry();
//    MT_RWMutexWriteLock lock(ntr.myMutex);
//
//#if MC_ENABLE_ASSERTS
//    MC_NodeType** ntptr = ntr.myTypesById.GetIfExists(aNodeType->GetId());
//    MC_ASSERT(!ntptr || *ntptr == aNodeType, MC_Strfmt<256>("NodeType [%s] was already registered.", aNodeType->GetId())); (void)ntptr;
//#endif
//
//    ntr.myTypesById[aNodeType->GetId()] = aNodeType;
//    ntr.myTypesByLabel[aNodeType->GetLabel()].AddNodeType(aNodeType);
//
//    int size = aNodeType->GetSize();
//    MC_ASSERT(size >= sizeof(MC_Node));
//    if (MC_NodeBySize* node = ntr.myNodesBySize.GetIfExistsFromKey(size))
//        ++node->myCount;
//    else
//    {
//        MC_NodeBySize nbs = { aNodeType->GetId(), size, 1 };
//        ntr.myNodesBySize.Insert(nbs);
//    }
//
//    theNodeTypeRegisteredCallback(aNodeType);
//}
//
//template<class Type>
//struct MC_DefaultNodeType : MC_NodeType, public MC_Noncopyable
//{
//    static MC_DefaultNodeType* ourInstance;
//
//    MC_DefaultNodeType(const char* anId, const char* aLabel, uint64 aContextMask, const char* aAdditionalSearchString)
//        : MC_NodeType(aContextMask), myId(anId), myLabel(aLabel)
//#if IS_EDITOR_BUILD
//        , myAdditionalSearchString(aAdditionalSearchString)
//#endif
//    {
//        MC_UNUSED(aAdditionalSearchString);
//        myIsStateless = sizeof(Type) == sizeof(typename Type::PinStorageBase);
//
//        assert(!ourInstance);
//        ourInstance = this;
//    }
//    ~MC_DefaultNodeType()
//    {
//        ourInstance = 0;
//    }
//
//    const char* GetId() const override { return myId; }
//    const char* GetLabel() const override { return myLabel; }
//    const char* GetNodeClassName() const override
//    {
//#if IS_EDITOR_BUILD && IS_PC_PLATFORM
//        return typeid(Type).name();
//#else
//        return nullptr;
//#endif
//    }
//
//#if IS_EDITOR_BUILD
//    const char* GetAdditionalSearchString(const char* /*aFilename*/ = "") const override { return myAdditionalSearchString; }
//#endif
//    int GetSize() const override { return sizeof(Type); }
//
//    MC_Node* CreateInstance(const MC_ReadOnlyVariantMap& /*aState*/) const override
//    {
//        return new Type;
//    }
//
//    const char* const myId;
//    const char* const myLabel;
//#if IS_EDITOR_BUILD
//    const char* const myAdditionalSearchString;
//#endif
//};
//
//template<class Type>
//MC_DefaultNodeType<Type>* MC_DefaultNodeType<Type>::ourInstance;
//
//template<class Type>
//MC_NodeType* MC_RegisterNodeType(const char* anId, const char* aLabel, uint64 aContextMask, const char* aAdditionalSearchString)
//{
//    MC_ASSERT(aLabel[0] != '<', "Are you trying to hide this node? Use SetHidden() instead.");
//    static MC_DefaultNodeType<Type> nodeType(anId, aLabel, aContextMask, aAdditionalSearchString);
//    MC_ASSERT(Type::NodeType() == nodeType.ourInstance, "Did you use MC_DERIVED_NODE correctly in your class? Static Type: [%s] Virtual Type [%s]",
//        Type::NodeType()->GetId(),
//        nodeType.ourInstance->GetId());
//    MC_ASSERT(MC_Strcmp(anId, nodeType.GetId()) == 0, "You are trying to register a node class with id [%s] again with a different id [%s]", nodeType.GetId(), anId);
//    MC_ERROR_IF(MC_Strcmp(aLabel, nodeType.GetLabel()) != 0, "Trying to register a node class (%s) with label [%s] again with a different label [%s]. This won't work.", nodeType.GetId(), nodeType.GetLabel(), aLabel);
//    MC_ERROR_IF(aContextMask != nodeType.GetContextMask(), "Trying to register a node class (%s) with ContextMask [%llu] again with a different ContextMask [%llu]. This won't work.", nodeType.GetId(), nodeType.GetContextMask(), aContextMask);
//#if IS_EDITOR_BUILD		// since the additionalSearchString does not get set outside of editor builds
//    MC_ERROR_IF(MC_Strcmp(aAdditionalSearchString, nodeType.GetAdditionalSearchString()) != 0, "Trying to register a node class (%s) with AdditionalSearchString [%s] again with a different AdditionalSearchString [%s]. This won't work.", nodeType.GetId(), nodeType.GetAdditionalSearchString(), aAdditionalSearchString);
//#endif
//    MC_RegisterNodeType(&nodeType);
//    return &nodeType;
//}
//
//
//struct MC_NodeTypeRegistry
//{
//    typedef MC_HashMap<const char*, MC_NodeType*> TypesById;
//    typedef MC_HashMap<const char*, MC_NodeTypesForLabel> TypesByLabel;
//    typedef MC_HashSet<MC_NodeBySize> NodesBySize;
//    typedef MC_GrowingArray<MC_CreateNodeFallbackFunc> CreateNodeFallbacks;
//
//    MT_ReadWriteMutex myMutex;
//    TypesById myTypesById;
//    TypesById myTypeReplacementOverrides;
//    TypesByLabel myTypesByLabel;
//    NodesBySize myNodesBySize;
//    CreateNodeFallbacks myCreateNodeFallbacks;
//};

#include "Gameplay/ECS/Components/ComponentType.h"

#include <unordered_map>
#include <unordered_set>


namespace ECS
{

struct ComponentRegistry
{
    using TypesById = std::unordered_map<const char*, const ComponentType*>;
    using IdToComponentTag = std::unordered_map<const char*, ComponentTag>;
    using IdToCreationFp = std::unordered_map<const char*, CreateComponentFP>;

    TypesById m_componentTypes;
    IdToComponentTag m_idToTag;
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
    return registery.m_componentTypes[componentName];
}

inline Component* CreateComponent(const char* componentName)
{
    auto& registery = GetComponentRegistry();
    return registery.m_idToCreationFp[componentName]();
}

}
