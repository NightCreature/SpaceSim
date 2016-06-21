#include "Graphics/MeshGroup.h"
#include "Application/BaseApplication.h"
#include "Graphics/DeviceManager.h"
#include "Core/Resource/GameResource.h"
#include "Graphics/RenderInstance.h"
#ifdef _DEBUG
#include "Core/StringOperations/StringHelperFunctions.h"
#endif

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
void MeshGroup::update( Resource* resource, RenderInstanceTree& renderInstance, float elapsedTime, const Matrix44& world, const std::string& name )
{
    if (m_renderInstanceDirty || m_renderInstance == nullptr)
    {  
        //if ( m_renderInstance != nullptr)
        //{
        //    delete m_renderInstance;
        //    m_renderInstance = nullptr;
        //}
		if (m_renderInstance == nullptr)
		{
			m_renderInstance = new RenderInstance(&m_geometryInstance, &m_shaderInstance);
#ifdef _DEBUG
			convertToWideString(name, m_renderInstance->m_name);
#else
			UNUSEDPARAM(name);
#endif
		}

        WVPBufferContent& wvpConstants = m_shaderInstance.getWVPConstants();
        wvpConstants.m_projection = Application::m_projection;
        wvpConstants.m_view = Application::m_view;
        wvpConstants.m_world = m_world * world; 
        UNUSEDPARAM(resource);
    }
    
    if (m_renderInstance != nullptr)
    {
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
