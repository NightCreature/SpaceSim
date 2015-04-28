#pragma once

#include "..\SpaceSim\RenderInstance.h"
#include "..\SpaceSim\MeshGroup.h"
#include "..\SpaceSim\Types.h"
#include "BBox.h"
#include <list>
#include <vector>

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

    void update( Resource* resource, RenderInstanceTree& renderInstance, float elapsedTime, const Matrix44& world, const std::string& name)
    {
        if (!m_modelData.empty())
        {
            for (size_t counter = 0; counter < m_modelData.size(); ++counter)
            {
                m_modelData[counter]->update(resource, renderInstance, elapsedTime, world, name);
            }
        }
    }

    void addMeshGroup(MeshGroup* meshGroup) { m_modelData.push_back(meshGroup); }
    const Bbox& getBoundingBox() const { return m_boundingBox; }
    Bbox& getBoundingBox() { return m_boundingBox; }
	const Bbox& getOriginalBoundingBox() const { return m_originalBBox; }
    Bbox& getOriginalBoundingBox() { return m_originalBBox; }
    void setOriginalBoundingBox(const Bbox& boundingBox) { m_originalBBox = boundingBox; }

    //This should indicate which mesh group it wants to set this material on
    void setMaterial( const Material& material) { m_modelData[0]->setMaterial( material ); }

    const std::vector<MeshGroup*>& getMeshData() const { return m_modelData; }
    std::vector<MeshGroup*>& getMeshData() { return m_modelData; }
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