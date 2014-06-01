#pragma once

#include "..\SpaceSim\RenderInstance.h"
#include "..\SpaceSim\MeshGroup.h"
#include "..\SpaceSim\Types.h"
#include "BBox.h"
#include <list>
#include <vector>

class Input;
class GameResource;
class Resource;

class Model
{
public:
    typedef Model Super;

    Model(Resource* resource) : m_resource(resource) {}
    virtual ~Model() 
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

    virtual void initialise(const ShaderInstance& shaderInstance) = 0;
    void update( RenderInstanceTree& renderInstance, float elapsedTime, const Matrix44& world, const std::string& name)
    {
        if (!m_modelData.empty())
        {
            for (size_t counter = 0; counter < m_modelData.size(); ++counter)
            {
                m_modelData[counter]->update(m_resource, renderInstance, elapsedTime, world, name);
            }
        }
    }

    void addMeshGroup(MeshGroup* meshGroup) { m_modelData.push_back(meshGroup); }
    const Bbox& getBoundingBox() const { return m_boundingBox; }
    Bbox& getBoundingBox() { return m_boundingBox; }
	const Bbox& getOriginalBoundingBox() const { return m_originalBBox; }
    Bbox& getOriginalBoundingBox() { return m_originalBBox; }

    //This should indicate which mesh group it wants to set this material on
    void setMaterial( const Material& material) { m_modelData[0]->setMaterial( material ); }

    const Resource& getResource() const { return *m_resource; }
    const GameResource& getGameResource() const { return *(GameResource*)m_resource; }

    const std::vector<MeshGroup*>& getMeshData() const { return m_modelData; }
protected:
    std::vector<MeshGroup*> m_modelData;
	Bbox m_originalBBox;
    Bbox m_boundingBox;
    Resource* m_resource;
};
