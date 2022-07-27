#pragma once

#include "Math/Vector3.h"
#include "Math/VectorTypes.h"

#include <unordered_map>
#include <vector>

template <class T>
class SpatialHashMap
{
public:
    using key = size_t;
    using value = T;

    SpatialHashMap() = default;
    ~SpatialHashMap() = default;

    void Initialise(size_t cellGranularity, const Vector2i64& worldMinBound, const Vector2i64& WorldMaxBound)
    {
        m_cellGranularity = cellGranularity;
        m_min = worldMinBound;
        m_max = WorldMaxBound;
    }

    size_t GetHash(const Vector3& worldPos)
    {
        Vector3i64 index(worldPos.x() * m_inverseCellSize.x(), worldPos.y() * m_inverseCellSize.y(), worldPos.z() * m_inverseCellSize.z());
    }

    void InsertElement(const value& element, const Vector2i64& min, const Vector2i64& max)
    {}
private:
    std::unordered_map<key, std::vector<value>> m_data;
    Vector3 m_cellSize;
    Vector3 m_inverseCellSize;
    Vector2i64 m_min;
    Vector2i64 m_max;
    size_t m_cellGranularity = 0;
    

};