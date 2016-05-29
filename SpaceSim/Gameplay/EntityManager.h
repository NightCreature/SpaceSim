#pragma once

#include <assert.h>
#include <deque>
#include <functional>
#include <vector>
//
//const size_t ENTITY_INDEX_BITS = 48;
//const size_t ENTITY_INDEX_MASK = ( (1ull << 48) ) - 1;
//
//const size_t ENTITY_GENERATION_BITS = 16;
//const size_t ENTITY_GENERATION_MASK = (1 << ENTITY_GENERATION_BITS) - 1;
//
//const unsigned int MINIMUM_FREE_INDICES = 1024;
//
//struct Entity
//{
//    Entity(size_t index, unsigned short generation) { id = (size_t)generation << ENTITY_INDEX_BITS | index; }
//    Entity() { id = (size_t)-1; } //Default constructor creates an invalid entity
//    size_t id;
//
//    size_t index() const { return id & ENTITY_INDEX_MASK; }
//    size_t generation() const { return (id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK; }
//};
//
//class EntityHasher
//{
//public:
//    size_t operator() (const Entity& entity) { return entity.id; }
//};
//
//inline bool operator== (const Entity& lhs, const Entity& rhs) { return lhs.id == rhs.id; }
//
class EntityManager
{
public:
//
//    //Need to pool create the free indices
    EntityManager()
    {
    }
//
//    Entity create()
//    {
//        size_t idx;
//        if (m_free_indices.size() > MINIMUM_FREE_INDICES)
//        {
//            idx = m_free_indices.front();
//            m_free_indices.pop_front();
//        }
//        else
//        {
//            m_generation.push_back(0);
//            idx = m_generation.size() - 1;
//            assert(idx < (1ull << ENTITY_INDEX_BITS));
//        }
//        return Entity(idx, m_generation[idx]);
//    }
//
//    bool alive(Entity e) const
//    {
//        return m_generation[e.index()] == e.generation();
//    }
//
//    void destroy(Entity e)
//    {
//        const size_t idx = e.index();
//        ++m_generation[idx];
//        m_free_indices.push_back(idx);
//    }
private:
    std::vector<unsigned short> m_generation;
    std::deque<size_t> m_free_indices;
};
//
