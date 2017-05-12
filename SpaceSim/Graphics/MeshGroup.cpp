#include "Graphics/MeshGroup.h"
#include "Application/BaseApplication.h"
#include "Graphics/DeviceManager.h"
#include "Core/Resource/GameResource.h"
#include "Graphics/RenderInstance.h"
#include "Gameplay/bbox.h"
#ifdef _DEBUG
#include "Core/StringOperations/StringHelperFunctions.h"
#endif
#include "Core/Profiler/ProfilerMacros.h"

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
MeshGroup::~MeshGroup()
{
    if (m_renderInstance != nullptr)
    {
        delete m_renderInstance;
        m_renderInstance = nullptr;
    }
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void MeshGroup::update( Resource* resource, RenderInstanceTree& renderInstance, float elapsedTime, const Matrix44& world, const Matrix44& view, const Matrix44& projection, const std::string& name, const Bbox& box )
{
    PROFILE_EVENT("MeshGroup::update", Aqua);
    if (m_renderInstanceDirty || m_renderInstance == nullptr)
    {  
        //if ( m_renderInstance != nullptr)
        //{
        //    delete m_renderInstance;
        //    m_renderInstance = nullptr;
        //}
		if (m_renderInstance == nullptr)
		{
            PROFILE_EVENT("MeshGroup::update::Allocation", Black);
			m_renderInstance = new RenderInstance(&m_geometryInstance, &m_shaderInstance);
#ifdef _DEBUG
			convertToWideString(name, m_renderInstance->m_name); 
#else
			UNUSEDPARAM(name);
#endif
		}

        WVPBufferContent& wvpConstants = m_renderInstance->GetShaderInstance().getWVPConstants();
        wvpConstants.m_projection = projection;
        wvpConstants.m_view = view;
        wvpConstants.m_world = m_world * world; 
        UNUSEDPARAM(resource);
    }
    
    if (m_renderInstance != nullptr)
    {
        m_renderInstance->setBoundingBox(box);
        renderInstance.emplace_back(m_renderInstance);
    }

    UNUSEDPARAM(elapsedTime);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
MeshGroup::MeshGroup( const MeshGroup& source )
{
    m_world = source.m_world;
    m_geometryInstance = source.getGeometryInstance();
    m_shaderInstance = source.getShaderInstance();
    m_renderInstance = nullptr;
    m_renderInstanceDirty = true;
}
