#include "Gameplay/plane.h"
#include "Gameplay/box.h"
#include "Math/matrix44.h"
#include "Math/vector4.h"
#include <cmath>
#include "Windows.h"

#include "Graphics/MeshComponentData.h"

#include "Physics/PhysicsManager.h"

#include "Core/MessageSystem/MessageQueue.h"
#include "Core/MessageSystem/GameMessages.h"
#include "Core/MessageSystem/RenderMessages.h"

int Plane::m_planeCount = 0;

//This needs to change so that it is a helper class that sets up a face correctly and then returns the face
Plane::Plane(Resource* resource, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, bool changeWindingOrder):
GameObject(resource),
m_changeWindingOrder(changeWindingOrder)
{
    std::stringstream str("");
    str << "plane_" << m_planeCount;
    ++m_planeCount;
    m_name = str.str();
    m_nameHash = hashString(m_name);
	m_fillx = false;
	m_filly = false;
	m_fillz = false;
	m_rows = 10;
	m_coloms = 10;
    m_invertNormal = false;
	Vector3 minimum = minv(minv(v1,v2), minv(v3,v4));
	Vector3 maximum = maxv(maxv(v1,v2), maxv(v3,v4));
	if (fabs(minimum.x() - maximum.x()) < v1.getEqualPrecision())
	{
		
		m_widthstartpos = minimum.z();
		m_heightstartpos = minimum.y();
		m_widthendpos = maximum.z();
		m_heightendpos = maximum.y();
		m_fillx = true;
		m_fillvalue = v1.x();
	}
	else if (fabs(minimum.y() - maximum.y()) < v1.getEqualPrecision())
	{
		m_widthstartpos = minimum.x();
		m_heightstartpos = minimum.z();
		m_widthendpos = maximum.x();
		m_heightendpos = maximum.z();
		m_filly = true;
		m_fillvalue = v1.y();		
	}
	else if (fabs(minimum.z() - maximum.z()) < v1.getEqualPrecision())
	{
		m_widthstartpos = minimum.x();
		m_heightstartpos = minimum.y();
		m_widthendpos = maximum.x();
		m_heightendpos = maximum.y();
		m_fillz = true;
		m_fillvalue = v1.z();
	}
    
    UNUSEDPARAM(changeWindingOrder);
    //getPlane().initialise(m_fillvalue, m_fillx, m_filly, m_fillz, changeWindingOrder);
    transform();
}


//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void Plane::initialise(const ShaderInstance& shaderInstance)
{
    Face::CreationParams params;
//    params.resource = m_resource;
//    params.shaderInstance = &shaderInstance;
    params.nrVerticesInX = m_rows;
    params.nrVerticesInY = m_coloms;
    params.width = m_widthendpos - m_widthstartpos;
    params.height = m_heightendpos - m_heightstartpos;
    params.fillx = m_fillx;
    params.filly = m_filly;
    params.fillz = m_fillz;
    params.fillvalue = m_fillvalue;
    params.invertNormal = m_invertNormal;
    params.changeWindingOrder = m_changeWindingOrder;
    params.m_materialParameters = m_materialParameters;
    //if (10 != m_rows && 10 != m_coloms)
    //{
    //    params.tesselate = false;
    //}
    //else
    {
        params.tesselate = true;
    }

    auto resource = GameResourceHelper(m_resource).getWriteableResource();
    MessageSystem::CreateRenderResource<Face::CreationParams> createPlaneModel = CREATERENDERRESOURCEMESSAGE(Face::CreationParams);
    createPlaneModel.SetData(params);
    createPlaneModel.SetGameObjectId(static_cast<size_t>(m_nameHash)); //Not super but should work for now
    resource.m_messageQueues->getUpdateMessageQueue()->addMessage(createPlaneModel); 

    Super::initialise(shaderInstance);
}

void Plane::transform()
{
	Matrix44 translated;
	Matrix44 rotated;
	//glPushMatrix();
	if (m_fillx)
	{
		translate(m_world, 0.0f, m_heightstartpos, m_widthstartpos);
	}
	else if (m_filly)
	{
		translate(translated, m_widthendpos, 0.0f, m_heightstartpos);
		rotate(rotated, Vector3::yAxis(), 90.0f);
		m_world = rotated * translated;
	}
	else if (m_fillz)
	{
		translate(translated, m_widthendpos, m_heightstartpos, 0.0f);
		rotate(rotated, Vector3::zAxis(), -90.0f);
		m_world = rotated * translated;
	}
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const ShaderInstance Plane::deserialise( const tinyxml2::XMLElement* node )
{
    ShaderInstance shaderInstance;
    for (const tinyxml2::XMLElement* childElement = node->FirstChildElement(); childElement != 0; childElement = childElement->NextSiblingElement())
    {
        unsigned int childElementHash = hashString(childElement->Value());
        if (Material::m_hash == childElementHash)
        {
            m_materialParameters = Material::GetMaterialParameters(childElement);
        }
    }
    return shaderInstance;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void Plane::update( RenderInstanceTree& renderInstances, float elapsedTime, const Input& input )
{
    Super::update(renderInstances, elapsedTime, input);
    Vector3 lowerleft, upperright;
    //m_plane.fillMiniMapVectors(lowerleft, upperright);
    //glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    //if (m_fillx)
    //{
    //	glBegin(GL_QUADS);
    //	(lowerleft.x() , lowerleft.y(), lowerleft.z());
    //	(lowerleft.x() , upperright.y(), lowerleft.z());
    //	(lowerleft.x() , upperright.y(), upperright.z());
    //	(lowerleft.x() , lowerleft.y(), upperright.z());
    //	glEnd();
    //}
    //else if (m_filly)
    //{
    //	glBegin(GL_QUADS);
    //	(lowerleft.x() , lowerleft.y(), lowerleft.z());
    //	(upperright.x() , lowerleft.y(), lowerleft.z());
    //	(upperright.x() , lowerleft.y(), upperright.z());
    //	(lowerleft.x() , lowerleft.y(), upperright.z());
    //	glEnd();
    //}
    //else if (m_fillz)
    //{
    //	glBegin(GL_QUADS);
    //	(lowerleft.x() , lowerleft.y(), lowerleft.z());
    //	(upperright.x() , lowerleft.y(), lowerleft.z());
    //	(upperright.x() , upperright.y(), lowerleft.z());
    //	(lowerleft.x() , upperright.y(), lowerleft.z());
    //	glEnd();
    //}
    MessageSystem::RenderInformation renderInfo;
    MessageSystem::RenderInformation::RenderInfo data;
    data.m_renderObjectid = m_renderHandle;
    data.m_gameobjectid = m_nameHash;
    data.m_world = m_world;
    data.m_name = m_name;
    renderInfo.SetData(data);
    m_resource->m_messageQueues->getUpdateMessageQueue()->addMessage(renderInfo);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void Plane::handleMessage( const MessageSystem::Message& msg )
{
    if (msg.getMessageId() == MESSAGE_ID(CreatedRenderResourceMessage))
    {
        const MessageSystem::CreatedRenderResourceMessage& renderResourceMsg = static_cast<const MessageSystem::CreatedRenderResourceMessage&>(msg);
        renderResourceMsg.GetData();
        m_renderHandle = renderResourceMsg.GetData()->m_renderResourceHandle;
        //Store the render object reference we get back and the things it can do

        //Register the bounding box with the physics
        //GameResourceHelper(m_resource).getWriteableResource().getPhysicsManager().AddColidableBbox(&(m_drawableObject->getBoundingBox()));

        m_initialisationDone = true;
    }
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void Plane::invertNormal()
{
    m_invertNormal = true;
}
