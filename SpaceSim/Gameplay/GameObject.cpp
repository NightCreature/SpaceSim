#include "GameObject.h"

#include "Core/MessageSystem/GameMessages.h"
#include "Core/MessageSystem/MessageQueue.h"
#include "Core/Resource/GameResource.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Loader/ModelLoaders/ModelLoader.h"
#include "Math/matrix44.h"
#include "Math/vector3.h"

#include <imgui.h>
#include <Core/tinyxml2.h>
#include "Math/MathUtilityFunctions.h"



void GameObject::deserialise(const tinyxml2::XMLElement* node)
{
    const tinyxml2::XMLAttribute* attribute = node->FindAttribute("name");
    if (attribute != nullptr)
    {
        m_name = attribute->Value();
        m_nameHash = Hashing::hashString(m_name);
    }

    for (const tinyxml2::XMLElement* element = node->FirstChildElement(); element != 0; element = element->NextSiblingElement())
    {
        auto typeHash = HashString(element->Value());
        if (Material::m_hash == typeHash)
        {
            DeserialiseMaterial(element);
        }
        else if (typeHash == "model"_hashstring)
        {
            DeserialiseModel(element);
        }
        else if ("orientation"_hashstring == typeHash)
        {
            DeserialiseOrientation(element);
        }
    }

    //This is starting from the root of the object
    DeserialiseInternal(node);
}

void GameObject::OnDebugImgui()
{
    if (ImGui::TreeNode(m_name.c_str()))
    {
        ImGui::CollapsingHeader("World Matrix");
        ImGui::InputFloat4("World[0]", m_world[0]);
        ImGui::InputFloat4("World[1]", m_world[1]);
        ImGui::InputFloat4("World[2]", m_world[2]);
        ImGui::InputFloat4("World[3]", m_world[3]);

        OnDebugImguiInternal();
        ImGui::TreePop();
    }
}

void GameObject::DeserialiseModel(const tinyxml2::XMLElement* element)
{
    const tinyxml2::XMLAttribute* attribute = element->FindAttribute("file");
    if (attribute != nullptr)
    {
        //Heavily relies on the shader instance existing before we load the model, might be better to put the model construction in initialise instead
        //m_drawableObject = getWriteableResource().getModelManager().LoadModel(m_resource, shaderInstance, attribute->Value());
        //m_drawableObject->setDirty();
        MSG_TRACE_CHANNEL_FMT("GameObject", "Create Render Object for file: {}", attribute->Value());

        auto resource = GameResourceHelper(m_resource).getWriteableResource();
        DECLAREANDCREATERESOURCEMESSAGE(createModel, LoadModelResource);
        LoadModelResource param;
        stringCopy(param.m_fileName, attribute->Value());
        createModel.SetData(param);
        createModel.SetGameObjectId(static_cast<size_t>(m_nameHash)); //Not super but should work for now
        resource.m_messageQueues->getUpdateMessageQueue()->addMessage(createModel);
    }
}

void GameObject::DeserialiseMaterial(const tinyxml2::XMLElement* element)
{

    //mat.deserialise(m_resource, element);
}

void GameObject::DeserialiseOrientation(const tinyxml2::XMLElement* element)
{
    Matrix44 transform;
    Matrix44 rotation;
    Matrix44 scaleMatrix;
    for (const tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement != 0; childElement = childElement->NextSiblingElement())
    {

        HashString elementName = HashString(element->Value());
        if (elementName == "position"_hashstring)
        {
            Vector3 position;
            position.deserialise(childElement);

            translate(transform, position);
        }
        else if (elementName == "direction"_hashstring)
        {
            Vector3 direction;
            direction.deserialise(childElement);
            direction.normalize();
            rotation = rotationFromDirection(direction);
        }
        else if (elementName == "rotation"_hashstring)
        {
            Vector4 axis = math::Deserialise(childElement);

            Vector3 axis3 = Vector3(axis.x(), axis.y(), axis.z());
            axis3.normalize(); //make sure the angle is a unit vector

            rotate(rotation, axis3, axis.w());
        }
        else if (elementName == "scale"_hashstring)
        {
            Vector3 scaleVector;
            scaleVector.deserialise(childElement);
            scale(scaleMatrix, scaleVector.x(), scaleVector.y(), scaleVector.z());
        }

    }

    m_world = scaleMatrix * rotation * transform;
}
