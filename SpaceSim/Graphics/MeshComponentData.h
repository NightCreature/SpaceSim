//#pragma once
//
//#include "Types.h"
//#include "EntityManager.h"
//
//#include <assert.h>
//#include <unordered_map>
//
//struct InstanceData {
//    size_t n;          ///< Number of used instances.
//    size_t allocated;  ///< Number of allocated instances.
//    void *buffer;        ///< Buffer with instance data.
//
//    Entity* m_entities;
//    Model* m_models;
//    Matrix44* m_worldMatrices;
//};
//
//
//class ModelComponentManger
//{
//
//public:
//    ModelComponentManger()
//    {
//        //allocate(32);
//    }
//
//    void allocate(size_t sz)
//    {
//        assert(sz > m_data.n);
//
//        InstanceData newdata;
//        const size_t bytes = sz * (sizeof(Entity) + sizeof(Model) + sizeof(Matrix44));
//        newdata.buffer = new char[bytes];//_allocator.allocate(bytes);
//        newdata.n = m_data.n;
//        newdata.allocated = sz;
//
//        newdata.m_entities = (Entity *)(newdata.buffer);
//        newdata.m_models = (Model*)(&newdata.m_entities[m_data.n]);
//        newdata.m_worldMatrices = (Matrix44*)(&newdata.m_models[m_data.n]);
//
//        memcpy(newdata.m_entities, m_data.m_entities, m_data.n * sizeof(Entity));
//        memcpy(newdata.m_models, m_data.m_models, m_data.n * sizeof(Model));
//        memcpy(newdata.m_models, m_data.m_worldMatrices, m_data.n * sizeof(Matrix44));
//
//        delete[] m_data.buffer;//_allocator.deallocate(m_data.buffer);
//        m_data = newdata;
//    }
//
//    void destroy(unsigned i)
//    {
//        size_t last = m_data.n - 1;
//        Entity e = m_data.m_entities[i];
//        Entity last_e = m_data.m_entities[last];
//
//        m_data.m_entities[i] = m_data.m_entities[last];
//        m_data.m_models[i] = m_data.m_models[last];
//        m_data.m_worldMatrices[i] = m_data.m_worldMatrices[last];
//
//        m_entitiesDataLookup[last_e] = i;
//        m_entitiesDataLookup.erase(e);
//
//        --m_data.n;
//    }
//
//    /// Handle to a component instance.
//    //Access helper so we only have to use the indirection map once for each entity
//    struct Instance 
//    {
//        static const size_t InvalidInstance = (size_t)-1;
//        size_t i; 
//    };
//
//    /// Returns the component instance for the specified m_entities or a nil instance
//    /// if the m_entities doesn't have the component.
//    Instance lookup(Entity e) 
//    {
//        if (m_entitiesDataLookup.find(e) != m_entitiesDataLookup.end())
//        {
//            return make_instance(m_entitiesDataLookup[e]);
//        }
//        
//        return make_instance((size_t)-1);
//    }
//
//    void CollectRenderInstances(Resource* resource, RenderInstanceTree& renderInstances)
//    {
//        //Loop through models and add it to the list to render
//        Matrix44 world;
//        world.identity();
//        for (size_t counter = 0; counter < m_data.n; ++counter)
//        {
//            m_data.m_models[counter].update(resource, renderInstances, 0.0f, m_data.m_worldMatrices[counter], "");
//        }
//    }
//
//    void addEntity(const Entity e, Model& model, const Matrix44& matrix)
//    {
//        Instance i = lookup(e);
//        if (i.i == Instance::InvalidInstance)
//        {
//            if (m_data.allocated == m_data.n)
//            {
//                //Need to reallocated the instance data
//                allocate(m_data.allocated + 32);
//            }
//
//            m_data.m_entities[m_data.n] = e;
//            m_data.m_models[m_data.n] = model;
//            m_data.m_worldMatrices[m_data.n] = matrix;
//            m_entitiesDataLookup[e] = m_data.n;
//            ++m_data.n;
//        }
//    }
//private:
//    /// Create an instance from an index to the data arrays.
//    Instance make_instance(size_t i) { Instance inst = { i }; return inst; }
//
//    std::unordered_map<Entity, size_t, EntityHasher> m_entitiesDataLookup;
//    InstanceData m_data;
//};