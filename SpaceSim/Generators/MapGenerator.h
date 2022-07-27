//#pragma once
//
//#include "Core/Thread/Job.h"
//#include "Generators/astar.h"
//#include "Graphics/Model.h"
//#include "Math/VectorTypes.h"
//
//#include <array>
//#include <atomic>
//#include <vector>
//
//
//
//#include "Core\Types\Types.h"
//#include "Gameplay\plane.h" //This would need a resource
//#include "Graphics\material.h"
//#include "Generators\astar.h"
//#include "Math\matrix44.h"
//
//#include "Math\MathUtilityFunctions.h"
//#include "Core\Resource\GameResource.h"
//#include "Core\Thread\Timer.h"
//
//
//class Resource;
//
//namespace SpaceSimulator
//{
//
//    class Entity;
//
//    struct RoomNode
//    {
//        RoomNode() = default;
//
//        void containsRoom(RoomNode& room)
//        {
//            if (room != *this)
//            {
//                Vector2s boundsMax = m_position + m_dimension;
//                if (m_position.Get(0) < room.m_position.Get(0) && room.m_position.Get(0) < boundsMax.Get(0) &&
//                    m_position.Get(1) < room.m_position.Get(1) && room.m_position.Get(1) < boundsMax.Get(1))
//                {
//                    m_connnected = true;
//                    room.m_connnected = true;
//                }
//            }
//
//            //Do nothing as we are comparing with ourselves
//        }
//
//        bool operator==(const RoomNode& rhs)
//        {
//            return m_position.equal(rhs.m_position) && m_dimension.equal(rhs.m_dimension) && m_connnected == rhs.m_connnected && m_numberOfConnections == rhs.m_numberOfConnections;
//        }
//
//        bool operator!=(const RoomNode& rhs)
//        {
//            return !(*this == rhs);
//        }
//
//        Vector2s m_position;
//        Vector2s m_dimension;
//        size_t m_numberOfConnections = 0;
//        bool m_connnected = false;
//    };
//
//    struct SearchNode
//    {
//        Vector2s m_position;
//        size_t m_distanceFromStartPoint = 0;
//    };
//
//    enum RoomType : uint8
//    {
//        eEmpty,
//        eRoom,
//        eCorridor,
//    };
//
//    template<size_t x, size_t y, size_t z>
//    struct FloorGenerationParameters
//    {
//        //FloorGenerationParameters(RoomType& map) : m_map(map) {}
//
//        Vector4s m_roomDimensionRange;
//        Vector3s m_floorDimensions;
//        RoomType* m_map[x][y][z];
//        size_t m_startFloor = 0;
//        size_t m_endFloor = 0;
//        size_t m_roomsPerFloor = 0;
//        size_t m_entitiesPerFloor = 0;
//        size_t m_connectingCorridorsPerFloor = 0;
//
//        std::atomic<size_t>* m_runningJobs = nullptr; //increment on start decrement on complete
//    };
//
//    struct CollisionNode
//    {
//        enum WallType : uint8
//        {
//            CEILING = 0,
//            FLOOR,
//            LEFTWALL,
//            RIGHTWALL,
//            FRONTWALL,
//            BACKWALL,
//
//            MAXWALLS
//        };
//
//        CollisionNode() = default;
//        ~CollisionNode() = default;
//
//        std::string ToString()
//        {
//            return std::string(m_walls[0] ? "[1" : "[0") + (m_walls[1] ? "1" : "0") + (m_walls[2] ? "1" : "0") + (m_walls[3] ? "1" : "0") + (m_walls[4] ? "1" : "0") + (m_walls[5] ? "1]" : "0]");
//        }
//
//        bool hasWall(WallType wall) { return m_walls[(int)wall]; }
//        void setWall(WallType wall, bool hasWall) { m_walls[(int)wall] = hasWall; }
//        std::array<bool, static_cast<std::underlying_type_t<WallType>>(WallType::MAXWALLS)> m_walls;
//    };
//
//    std::vector<SpaceSimulator::AStarNode*> findNeighbours(const SpaceSimulator::AStarNode& node, const std::vector<SpaceSimulator::AStarNode>& nodeMap, size_t maxWidth, size_t maxHeight);
//    inline float distance(const SpaceSimulator::AStarNode& current, const SpaceSimulator::AStarNode& node)
//    {
//        return static_cast<float>(std::abs(static_cast<long long>(current.m_nodeValue.Get(0) - node.m_nodeValue.Get(0))) + std::abs(static_cast<long long>(current.m_nodeValue.Get(1) - node.m_nodeValue.Get(1))));
//    }
//    inline float heuristic(const SpaceSimulator::AStarNode& node, const SpaceSimulator::AStarNode& goal)
//    {
//        return 0.9f * (std::abs(static_cast<long long>(node.m_nodeValue.Get(0) - goal.m_nodeValue.Get(0))) + std::abs(static_cast<long long>(node.m_nodeValue.Get(1) - goal.m_nodeValue.Get(1)))) * 0.99999f;
//    }
//
//    template<size_t x, size_t y, size_t z>
//    class MapLayerJob : public Job
//    {
//    public:
//        MapLayerJob(const FloorGenerationParameters<x, y, z>& params) : m_parameters(params) {}
//        void Execute(size_t threadIndex) override;
//    private:
//
//        void generateRooms(size_t levelCounter, std::vector<RoomNode>& m_rooms);
//        void setConnectionOnOverlappingRooms(std::vector<RoomNode>& m_rooms);
//
//        /// <summary>
//        /// Costly part of the generation as we need to walk through the array in a non ordered manner, sufficiently large maps ask for threading this
//        /// </summary>
//        void connectRooms(size_t levelCounter, std::vector<RoomNode>& m_rooms);
//        void generateEntitiesForLevel(size_t levelCounter, size_t entitiesPerLevel, std::vector<RoomNode>& m_rooms, std::vector<Entity>& entitiesList);
//
//        FloorGenerationParameters<x, y, z> m_parameters;
//    };
//
//    /// <summary>
//    /// Class generates random maps
//    /// </summary>
//    template<size_t x, size_t y, size_t z>
//    class MapGenerator
//    {
//    public:
//        MapGenerator() {}
//        //MapGenerator(Vector3 mapDimensions, Vector3 roomDimensions, int roomsPerFloor, int floorConnectionsPerFloor, int maxNumberRooms)
//        /// <summary>
//        /// Constructor for random map generator
//        /// </summary>
//        /// <param name="mapDimensions">Map dimensions specified in blocks, each block is a 50,50,50 cube in the world</param>
//        /// <param name="roomDimensionsRange">Range of the dimensions for the rooms, x,y: horizontal block range, z,w: vertical block range</param>
//        /// <param name="roomsPerFloor">Number of rooms per floor</param>
//        /// <param name="connectingCooridorsPerFloor">Number of corridors that connect rooms per floor</param>
//        /// <param name="connectingCorridorsBetweenFloors">Number of corridors between floors</param>
//        void Initialise(size_t numberOfThreadsToSpawn, const Vector4s& roomDimensionsRange, int roomsPerFloor, size_t connectingCooridorsPerFloor, size_t connectingCorridorsBetweenFloors);
//
//        bool generateMap(Resource* resource);
//
//        void Update();
//    private:
//        CreatedModel generateGeometry();
//        //void generateCollisionMeshesForLevel(Entity& entity);
//
//        RoomType m_map[x][y][z];
//        Vector4s m_roomDimensionRange;
//
//        size_t m_numberOfThreadsToSpawn = 1;
//        size_t m_roomsPerFloor = 0;
//        size_t m_connectingCorridorsPerFloor = 0;
//        size_t m_connectingCorridorsBetweenFloors = 0;
//
//        RoomNode m_startPosition;
//        RoomNode m_endPosition;
//
//        std::vector<MapLayerJob<x, y, z>> m_jobs;
//
//        std::atomic<size_t> m_runningJobs = 0;
//
//        size_t timestamp = 0;
//    };
//
//#include "MapGenerator.inl"
//
//}
