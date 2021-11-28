#pragma once

#include "Graphics/RenderInstance.h"
#include "Graphics/MeshGroup.h"
#include "Core/Types\Types.h"
#include "Gameplay/BBox.h"
#include <list>
#include <vector>

#include <D3D12.h>

class Input;
class Resource;

class Model
{
public:
    struct CreationParams
    {
        Resource* m_resource;
        const ShaderInstance* m_shaderInstance;
    };

    Model() {}
    ~Model() 
    {
        if (m_modelData.size() > 0)
        {
            for (auto it = m_modelData.begin(); it != m_modelData.end(); ++it)
            {
                delete *it;
            }
            m_modelData.clear();
        }
    }

    void update( Resource* resource, RenderInstanceTree& renderInstance, float elapsedTime, const Matrix44& world, const Matrix44& view, const Matrix44& projection, const std::string& name)
    {
        if (!m_modelData.empty())
        {
            for (size_t counter = 0; counter < m_modelData.size(); ++counter)
            {
                m_modelData[counter]->update(resource, renderInstance, elapsedTime, world, view, projection, name, m_boundingBox);
            }
        }
    }

    ///-----------------------------------------------------------------------------
    ///! @brief   
    ///! @remark Since View and porjections are mostly per frame or per pass constants we dont need to set them
    ///-----------------------------------------------------------------------------
    void Update(Resource* resource, CommandList& list, float elapsedTime, const Matrix44& world, const std::string& name)
    {
        if (!m_modelData.empty())
        {
            for (size_t counter = 0; counter < m_modelData.size(); ++counter)
            {
                m_modelData[counter]->Update(resource, list, elapsedTime, world, name, m_boundingBox);
            }
        }
    }

    void addMeshGroup(MeshGroup* meshGroup) { m_modelData.push_back(meshGroup); }
    const Bbox& getBoundingBox() const { return m_boundingBox; }
    Bbox& getBoundingBox() { return m_boundingBox; }
	const Bbox& getOriginalBoundingBox() const { return m_originalBBox; }
    Bbox& getOriginalBoundingBox() { return m_originalBBox; }
    void setOriginalBoundingBox(const Bbox& boundingBox) { m_originalBBox = boundingBox; }
    void setBoundingBox(const Bbox& boundingBox) { m_boundingBox = boundingBox; }

    //This should indicate which mesh group it wants to set this material on
    void setMaterial( const Material& material) { m_modelData[0]->setMaterial( material ); }

    const std::vector<MeshGroup*>& getMeshData() const { return m_modelData; }
    std::vector<MeshGroup*>& getMeshData() { return m_modelData; }

    void setDirty()
    {
        for (auto meshGroup : m_modelData)
        {
            meshGroup->setDirty();
        }
    }

    void setShaderInstance(const ShaderInstance& shaderInstance)
    {
        for (auto meshGroup : m_modelData)
        {
            meshGroup->setShaderInstance(shaderInstance);
        }
    }
protected:
    std::vector<MeshGroup*> m_modelData;
    Bbox m_originalBBox;
    Bbox m_boundingBox;
};

struct CreatedModel
{
    Model* model;
    Bbox boundingBox;
};

struct CreatedMeshGroup
{
    MeshGroup* meshGroup;
    Bbox boundingBox;
};