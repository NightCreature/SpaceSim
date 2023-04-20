#pragma once

#include "Graphics/RenderInstance.h"
#include "Graphics/MeshGroup.h"
#include "Core/Types\Types.h"
#include "Gameplay/BBox.h"
#include "EffectCache.h"
#include <list>
#include <vector>

#include <D3D12.h>
#include <Optick.h>
#include "Graphics/RenderInterface.h"


class Input;
class Resource;

class Model : public RenderInterface
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
        for (auto& meshGroup : m_modelData)
        {
            meshGroup.Cleanup();
        }

    }

    void update( Resource* resource, RenderInstanceTree& renderInstance, float elapsedTime, const Matrix44& world, const Matrix44& view, const Matrix44& projection, const std::string& name)
    {
        if (!m_modelData.empty())
        {
            for (size_t counter = 0; counter < m_modelData.size(); ++counter)
            {
                m_modelData[counter].update(resource, renderInstance, elapsedTime, world, view, projection, name, m_boundingBox);
            }
        }
    }

    ///-----------------------------------------------------------------------------
    ///! @brief   
    ///! @remark Since View and porjections are mostly per frame or per pass constants we dont need to set them
    ///-----------------------------------------------------------------------------
    void Update(float elapsedTime, const Matrix44& world, const std::string& name)
    {
        UNUSEDPARAM(elapsedTime);

        if (!m_modelData.empty())
        {
            for (size_t counter = 0; counter < m_modelData.size(); ++counter)
            {
                m_modelData[counter].Update(world, name, m_boundingBox);
            }
        }
    }

    void Update(const MessageSystem::RenderInformation::RenderInfo& context)
    {
        if (!m_modelData.empty())
        {
            for (size_t counter = 0; counter < m_modelData.size(); ++counter)
            {
                m_modelData[counter].Update(context);
            }
        }
    }

    void PopulateCommandlist(Resource* resource, CommandList& commandList) override
    {
        OPTICK_EVENT();
        if (!m_modelData.empty())
        {
            for (auto& group : m_modelData)
            {
                group.PopulateCommandlist(resource, commandList);
            }
        }
    }

    void UpdateCbs() {}

    //Deprecated bad code
    void addMeshGroup(MeshGroup* meshGroup) { m_modelData.push_back(*meshGroup); }
    const Bbox& getBoundingBox() const { return m_boundingBox; }
    Bbox& getBoundingBox() { return m_boundingBox; }
	const Bbox& getOriginalBoundingBox() const { return m_originalBBox; }
    Bbox& getOriginalBoundingBox() { return m_originalBBox; }
    void setOriginalBoundingBox(const Bbox& boundingBox) { m_originalBBox = boundingBox; }
    void setBoundingBox(const Bbox& boundingBox) { m_boundingBox = boundingBox; }

    //This should indicate which mesh group it wants to set this material on
    void setMaterial( const Material& material) { m_modelData[0].setMaterial( material ); }

    const std::vector<MeshGroup>& getMeshData() const { return m_modelData; }
    std::vector<MeshGroup>& getMeshData() { return m_modelData; }

    void setDirty()
    {
        for (auto& meshGroup : m_modelData)
        {
            meshGroup.setDirty();
        }
    }

    void setShaderInstance(const ShaderInstance& shaderInstance)
    {
        UNUSEDPARAM(shaderInstance);
        //for (auto& meshGroup : m_modelData)
        //{
        //    //meshGroup.setShaderInstance(shaderInstance);
        //}
    }

    MeshGroup& CreateMeshGroup()
    {
        m_modelData.emplace_back(MeshGroup({}));
        return *(--m_modelData.end());
    }

    uint64 GetSortKey() const { return m_sortKey; }
    void CalculateSortKey(const EffectCache& effectCache)
    {
        bool hasAlphaBlending = false;
        //Might want to order this internally in the meshgroup array based on blending have to see
        for (auto& meshGroup : m_modelData)
        {
            auto& material = meshGroup.GetMaterial();
            const Effect* effect = effectCache.getEffect(material.getEffectHash());
            if (effect != nullptr)
            {
                m_sortKey |= effect->getTechnique(material.getTechnique())->getTechniqueId();
                hasAlphaBlending |= material.getBlendState();
            }
        }

        if (hasAlphaBlending)
        {
            //we want these to be last in the list so set the first bit to 1
            m_sortKey |= (1ull << 63);
        }
    }


protected:
    std::vector<MeshGroup> m_modelData; //Why is meshgroup a pointer here and not just owned?
    Bbox m_originalBBox;
    Bbox m_boundingBox;
    uint64 m_sortKey = 0;
};

struct CreatedModel
{
    Model* model = nullptr;
    Bbox boundingBox;
};

//This should be different
struct CreatedMeshGroup
{
    MeshGroup* meshGroup = nullptr;
    Bbox boundingBox;
};