#include "LevelLoader.h"

#include "Gameplay/EntityManager.h"
#include "Gameplay/ECS/Components/ComponentTypeRegistry.h"
#include "Gameplay/Level.h"
#include "Core/Resource/GameResource.h"
#include "Core/Serialization/Archive.h"
#include "Core/tinyxml2.h"

#include "Graphics/light.h"
#include "Graphics/LightManager.h"
#include "Graphics/Material.h"
#include "Core/MessageSystem/MessageObserver.h"



Level LevelLoader::LoadLevel(const std::filesystem::path& filename, MessageSystem::MessageObserver& messageObservers)
{
    Level level;

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError error = doc.LoadFile(filename.string().c_str());
    if (error != tinyxml2::XML_SUCCESS)
    {
        MSG_ERROR_CHANNEL_FMT("LevelLoader", "Failed to load level file: {}", filename.string());
        return level;
    }

    const tinyxml2::XMLElement* root = doc.RootElement();
    if (root == nullptr)
    {
        MSG_ERROR_CHANNEL_FMT("LevelLoader", "Failed to find root element in level file");
        return level;
    }

    GameResourceHelper gameResourceHelper(m_resource);
    EntityManager& entityManager = gameResourceHelper.getWriteableResource().getEntityManager();
    
    messageObservers.AddDispatchFunction(MESSAGE_ID(AddRenderObjectMessage), fastdelegate::MakeDelegate(m_level, &Level::dispatchMessage));


    for (const tinyxml2::XMLElement* element = root->FirstChildElement(); element != 0; element = element->NextSiblingElement())
    {
        auto typeHash = HashString(element->Value());
        if (typeHash == "Lights"_hashstring)
        {
            //Levels list of lights will be loaded here
            for (const tinyxml2::XMLElement* lightElement = element->FirstChildElement(); lightElement != 0; lightElement = lightElement->NextSiblingElement())
            {
                Light light;
                light.deserialise(lightElement);
                const tinyxml2::XMLAttribute* attribute = lightElement->FindAttribute("name");
                if (attribute)
                {
                    level.AddLight(attribute->Value());
                }
                //These should be added to the light manager, level might know about the HashStrings of the names
            }
        }
        else if (typeHash == "Materials"_hashstring)
        {
            //Levels list of materials will be loaded here
            for (const tinyxml2::XMLElement* materialElement = element->FirstChildElement(); materialElement != 0; materialElement = materialElement->NextSiblingElement())
            {
                auto materialParams = Material::GetMaterialParameters(materialElement);
                level.AddMaterial(materialParams);
            }
        }
        else if (typeHash == "Entities"_hashstring)
        {
            //Levels list of Entities will be loaded here
            for (const tinyxml2::XMLElement* entityElement = element->FirstChildElement(); entityElement != 0; entityElement = entityElement->NextSiblingElement())
            {
                ECS::Entity& entity = entityManager.Create();
                std::string name;
                //Grab the data from the entity element
                for (const tinyxml2::XMLAttribute* attribute = entityElement->FirstAttribute(); attribute != 0; attribute = attribute->Next())
                {
                    const std::string_view attributeName(attribute->Name());
                    const std::string_view attributeValue(attribute->Value());
                    if (attributeName == "name")
                    {
                        name = attributeValue;
                    }

                    //debug out attributes
                    MSG_TRACE_CHANNEL_FMT("LevelLoader", "Entity Attribute: {} = {}", attributeName, attributeValue);
                }

                //Iterate over the sub elements and add the components to the entity
                for (const tinyxml2::XMLElement* componentElement = entityElement->FirstChildElement(); componentElement != 0; componentElement = componentElement->NextSiblingElement())
                {
                    const std::string_view componentName(componentElement->Value());
                    const ECS::ComponentType* componentType = ECS::GetComponentTypeForComponent(componentName);
                    if (componentType == nullptr)
                    {
                        MSG_ERROR_CHANNEL_FMT("LevelLoader", "Failed to find component type: {}", componentName);
                        continue;
                    }

                    ECS::Component& component = entity.AddComponent(componentType);
                    component.Deserialise(componentElement);
                }

                level.AddEntity(name, entity.GetId());
            }
        }
    }

    level.ProcessDataAndRequestResources();

    return level;
}


Level LevelLoader::LoadLevel(const std::filesystem::path& filename, MessageSystem::MessageObserver& messageObservers, std::true_type /*archive*/)
{
    //Level level;
    //Archive archive;
    //if (!archive.Open(filename))
    //{
    //    MSG_ERROR_CHANNEL_FMT("LevelLoader", "Failed to open level file: {}", filename.string());
    //    return level;
    //}

    //GameResourceHelper gameResourceHelper(m_resource);
    //EntityManager& entityManager = gameResourceHelper.getWriteableResource().getEntityManager();

    //

    //// Assuming you have a way to serialize and deserialize your lights, materials, and entities
    //// directly with the Archive class. You might need to adjust these parts based on your actual
    //// serialization/deserialization implementation.

    //// Read lights
    //std::vector<Light> lights;
    //archive.ReadContainer(lights);
    //for (const Light& light : lights)
    //{
    //    level.AddLight(light);
    //}

    //// Read materials
    //std::vector<Material::MaterialParameters> materials;
    //archive.ReadContainer(materials);
    //for (const Material::MaterialParameters& material : materials)
    //{
    //    level.AddMaterial(material);
    //}

    //// Read entities
    //size_t entityCount;
    //archive.Read(entityCount); // Assuming you have stored the count of entities
    //for (size_t i = 0; i < entityCount; ++i)
    //{
    //    ECS::Entity& entity = entityManager.Create();
    //    size_t componentCount;
    //    archive.Read(componentCount); // Assuming you have stored the count of components for each entity
    //    for (size_t j = 0; j < componentCount; ++j)
    //    {
    //        std::string componentName;
    //        archive.Read(componentName); // Assuming component names are stored as strings
    //        const ECS::ComponentType* componentType = ECS::GetComponentTypeForComponent(componentName);
    //        if (componentType == nullptr)
    //        {
    //            MSG_ERROR_CHANNEL_FMT("LevelLoader", "Failed to find component type: {}", componentName);
    //            continue;
    //        }

    //        ECS::Component& component = entity.AddComponent(componentType);
    //        //archive.Read(component); // Assuming components can be directly deserialized
    //        //level.AddEntity(entity.GetId());
    //    }
    //}

    //archive.Close();

    //return level;
}


void LevelLoader::SaveLevel(const std::filesystem::path& filename, const Level& level)
{
   
}

void LevelLoader::SaveLevel(const std::filesystem::path& filename, const Level& level, std::true_type /*archive*/)
{
    Archive archive;
    if (!archive.Open(filename))
    {
        MSG_ERROR_CHANNEL_FMT("LevelLoader", "Failed to open level file for saving: {}", filename.string());
        return;
    }

    // Assuming you have a way to serialize your lights, materials, and entities
    // directly with the Archive class. You might need to adjust these parts based on your actual
    // serialization/deserialization implementation.

    // Write lights
    archive.WriteContainer(level.GetLights());

    // Write materials
    archive.WriteContainer(level.GetMaterials());

    // Write entities
    // For entities, you might need to first write the count, then iterate through each entity
    // and its components to serialize them.
    GameResourceHelper gameResourceHelper(m_resource);
    EntityManager& entityManager = gameResourceHelper.getWriteableResource().getEntityManager();
    const auto& entityIds = level.GetEntities();
    size_t entityCount = entityIds.size();
    archive.Write(entityCount);

    for (const auto& entityId : entityIds)
    {
        // Assuming each entity has a method to serialize itself or you have a global function to do so.
        // Here, we're just writing the count of components as an example.
        //ECS::Entity entity;
        //if ( entityManager.GetEntity(entityId, entity))
        //{ }
        //const auto& components = entity.GetComponents();
        //size_t componentCount = components.size();
        //archive.Write(componentCount);

        //for (const auto& component : components)
        //{
        //    // You need to serialize each component here.
        //    // This could involve writing the component type name and then the component data.
        //    std::string componentName = component->GetComponentName(); // Assuming such a method exists
        //    archive.Write(componentName);
        //    archive.Write(*component); // Assuming components are serializable with the Archive
        //}
    }

    archive.Close();
}


