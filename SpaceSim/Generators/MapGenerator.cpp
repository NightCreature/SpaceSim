//#include "MapGenerator.h"
//
//#include <random>
//
//std::random_device randomDevice;
//std::mt19937_64 generator(randomDevice());
//
//
//namespace SpaceSimulator
//{
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//template<size_t x, size_t y, size_t z>
//void MapLayerJob<x, y, z>::Execute(size_t threadIndex)
//{
//    UNUSEDPARAM(threadIndex);
//
//    for (size_t levelCounter = m_parameters.m_startFloor; levelCounter < m_parameters.m_endFloor; ++levelCounter)
//    {
//        //#if PROFILE
//        //                DateTime intermediateStartTime = DateTime.Now;
//        //#endif
//        std::vector<RoomNode> rooms;
//        generateRooms(levelCounter, rooms);
//        setConnectionOnOverlappingRooms(rooms);
//        connectRooms(levelCounter, rooms);
//        //generateEntitiesForLevel(levelCounter, param.entitiesPerFloor, rooms);
//        //#if PROFILE
//        //                intermediateTimes[levelCounter] = (DateTime.Now - intermediateStartTime);
//        //#endif
//
//        //size_t widthPerLayer = m_parameters.m_floorDimensions.Get(0);
//        //size_t layerSize = widthPerLayer * m_parameters.m_floorDimensions.Get(1);
//        std::stringstream output;
//        output << "\n";
//
//        RoomType*** actualMap = reinterpret_cast<RoomType***>(m_parameters.m_map);
//        for (size_t xCounter = 0; xCounter < m_parameters.m_floorDimensions.Get(0); ++xCounter)
//        {
//            for (size_t zCounter = 0; zCounter < m_parameters.m_floorDimensions.Get(2); ++zCounter)
//            {
//
//                output << "[";
//                auto roomType = actualMap[xCounter][levelCounter][zCounter];
//                if (roomType == RoomType::eEmpty)
//                {
//                    output << "0";
//                }
//                else if (roomType == RoomType::eRoom)
//                {
//                    output << "R";
//                }
//                else
//                {
//                    output << "C";
//                }
//                output << "]";
//            }
//
//            output << "\n";
//        }
//
//        MSG_TRACE("Layer %d layout:", levelCounter);
//        OutputDebugString(output.str().c_str());
//    }
//
//    (*m_parameters.m_runningJobs)--;
//}
//
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//template<size_t x, size_t y, size_t z>
//void MapLayerJob<x, y, z>::generateRooms(size_t levelCounter, std::vector<RoomNode>& m_rooms)
//{
//    std::uniform_int_distribution<size_t> randomXRoomCooridnateDistrubution(m_parameters.m_roomDimensionRange.Get(0), m_parameters.m_roomDimensionRange.Get(1));
//    std::uniform_int_distribution<size_t> randomZRoomCooridnateDistrubution(m_parameters.m_roomDimensionRange.Get(2), m_parameters.m_roomDimensionRange.Get(3));
//
//    int numberOfRoomsGenerated = 0;
//    size_t widthPerLayer = m_parameters.m_floorDimensions.Get(0);
//    RoomType*** actualMap = reinterpret_cast<RoomType***>(m_parameters.m_map);
//    while (numberOfRoomsGenerated < m_parameters.m_roomsPerFloor)
//    {
//        auto room = RoomNode();
//        //Next(x, y) is get me a random number between x and y
//        auto randomNumber = generator();
//        auto rangedRandomNumber = (randomNumber % (m_parameters.m_roomDimensionRange.Get(1) + 1 - m_parameters.m_roomDimensionRange.Get(0))) + m_parameters.m_roomDimensionRange.Get(0);
//        auto x1 = rangedRandomNumber;// math::clamp(generator() % m_parameters.m_roomDimensionRange.Get(1), m_parameters.m_roomDimensionRange.Get(0), m_parameters.m_roomDimensionRange.Get(1));
//        auto y1 = math::clamp(generator() % m_parameters.m_roomDimensionRange.Get(3), m_parameters.m_roomDimensionRange.Get(2), m_parameters.m_roomDimensionRange.Get(3));
//        std::array<size_t, 2> coordinates = { x1, y1 };
//        room.m_dimension = Vector2s(coordinates);// m_randomXRoomSize.Next(, m_randomZRoomSize.Next(static_cast<size_t>(m_roomDimensionRange.Z, static_cast<size_t>(m_roomDimensionRange.W));
//
//        std::uniform_int_distribution<size_t> randomRoomPositionXDistrubution(0, widthPerLayer - room.m_dimension.Get(0));
//        std::uniform_int_distribution<size_t> randomRoomPositionZDistrubution(0, m_parameters.m_floorDimensions.Get(2) - room.m_dimension.Get(1));
//        std::array<size_t, 2> position = { randomRoomPositionXDistrubution(generator), randomRoomPositionZDistrubution(generator) };
//        room.m_position = Vector2s(position);// m_roomPosition.Next(static_cast<size_t>((m_mapDimensions.X - room.m_dimension.x)), m_roomPosition.Next(static_cast<size_t>((m_mapDimensions.Z - room.m_dimension.y))); 
//        room.m_connnected = false;
//
//        size_t upperRoomXOffset = room.m_position.Get(0) + room.m_dimension.Get(0);
//        size_t upperRoomZOffset = room.m_position.Get(1) + room.m_dimension.Get(1);
//        for (size_t x = room.m_position.Get(0); x < upperRoomXOffset; ++x)
//        {
//            for (size_t y = room.m_position.Get(1); y < upperRoomZOffset; ++y)
//            {
//                actualMap[x][levelCounter][y] = RoomType::eRoom;
//            }
//        }
//        m_rooms.push_back(room); //Build a list of all rooms so we can connect them later on
//        ++numberOfRoomsGenerated;
//    }
//
//    //if (levelCounter == 0)
//    //{
//    //    m_parameters.m_startPosition = (RoomNode)m_rooms[0];
//    //}
//    //if (levelCounter == m_parameters.m_floorDimensions.Get(1))
//    //{
//    //    m_parameters.m_endPosition = static_cast<RoomNode>(m_rooms[m_rooms.size() - 1]);
//    //}
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//template<size_t x, size_t y, size_t z>
//void MapLayerJob<x, y, z>::setConnectionOnOverlappingRooms(std::vector<RoomNode>& m_rooms)
//{
//    for (RoomNode& sourceRoom : m_rooms)
//    {
//        for (RoomNode& room : m_rooms)
//        {
//            sourceRoom.containsRoom(room);
//        }
//    }
//}
//
//std::vector<AStarNode*> findNeighbours(const AStarNode& node, const std::vector<AStarNode>& nodeMap, size_t maxWidth, size_t maxHeight)
//{
//    std::vector<AStarNode*> returnValue;
//    //Allow for 4 directions only so x-1,
//    size_t x = node.m_nodeValue.Get(0);
//    size_t y = node.m_nodeValue.Get(1);
//    if (x <= 0) //Left edge so no left node as a neighbour
//    {
//        returnValue.push_back(const_cast<AStarNode*>(&nodeMap[x + 1 + maxWidth * y]));
//    }
//    else if (x >= maxWidth - 1) //Right edge so no right neighbour
//    {
//        returnValue.push_back(const_cast<AStarNode*>(&nodeMap[x - 1 + maxWidth * y]));
//    }
//    else
//    {
//        returnValue.push_back(const_cast<AStarNode*>(&nodeMap[x + 1 + maxWidth * y]));
//        returnValue.push_back(const_cast<AStarNode*>(&nodeMap[x - 1 + maxWidth * y]));
//    }
//
//    if (y <= 0) //Top edge so no left node as a neighbour
//    {
//        returnValue.push_back(const_cast<AStarNode*>(&nodeMap[x + maxWidth * y + 1]));
//    }
//    else if (y >= maxHeight - 1) //Bottom edge so no right neighbour
//    {
//        returnValue.push_back(const_cast<AStarNode*>(&nodeMap[x + maxWidth * y - 1]));
//    }
//    else
//    {
//        returnValue.push_back(const_cast<AStarNode*>(&nodeMap[x + maxWidth * y + 1]));
//        returnValue.push_back(const_cast<AStarNode*>(&nodeMap[x + maxWidth * y - 1]));
//    }
//    return returnValue;
//}
//
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//template<size_t x, size_t y, size_t z>
//void MapLayerJob<x, y, z>::connectRooms(size_t levelCounter, std::vector<RoomNode>& m_rooms)
//{
//    size_t maxWidth = m_parameters.m_floorDimensions.Get(0);
//    RoomType*** actualMap = reinterpret_cast<RoomType***>(m_parameters.m_map);
//    //Should use the Astar utility here to find connections need to construct a grid of nodes first
//    for (size_t roomCounter = 0; roomCounter < m_rooms.size() - 1 && roomCounter < m_parameters.m_connectingCorridorsPerFloor; roomCounter += 2) //Dont connect previously connected rooms
//    {
//        RoomNode start = (RoomNode)m_rooms[roomCounter];
//        RoomNode end = (RoomNode)m_rooms[roomCounter + 1];
//
//        std::vector<AStarNode> astarMap;
//
//        astarMap.resize(m_parameters.m_floorDimensions.Get(0) * m_parameters.m_floorDimensions.Get(2));
//        for (size_t height = 0; height < m_parameters.m_floorDimensions.Get(2); ++height)
//        {
//            for (size_t width = 0; width < maxWidth; ++width)
//            {
//                std::array<size_t, 2> position = { width, height };
//                astarMap[width + maxWidth * height].m_nodeValue = Vector2s(position);
//            }
//        }
//
//        std::array<size_t, 2> dimension = { m_parameters.m_floorDimensions.Get(0), m_parameters.m_floorDimensions.Get(2) };
//        AStar astar = AStar(astarMap, Vector2s(dimension), start.m_position, end.m_position);
//        astar.m_heuristicFunc = &heuristic;
//        astar.m_findNeighboursFunc = findNeighbours;
//        astar.m_distanceFunc = distance;
//
//        if (astar.doAStar())
//        {
//            //Found valid path so set nodes to be rooms so it is connected
//            std::vector<AStarNode*> path = astar.Path();
//            for (const AStarNode* node : path)
//            {
//                //check this indexing into a cube
//
//                if (actualMap[node->m_nodeValue.Get(0)][levelCounter][node->m_nodeValue.Get(1)] == RoomType::eEmpty)
//                {
//                    actualMap[node->m_nodeValue.Get(0)][levelCounter][node->m_nodeValue.Get(1)] = RoomType::eCorridor;
//                }
//            }
//        }
//    }
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//template<size_t x, size_t y, size_t z>
//void MapLayerJob<x, y, z>::generateEntitiesForLevel(size_t levelCounter, size_t entitiesPerLevel, std::vector<RoomNode>& m_rooms, std::vector<Entity>& entitiesList)
//{
//    UNUSEDPARAM(levelCounter);
//    UNUSEDPARAM(entitiesPerLevel);
//    UNUSEDPARAM(m_rooms);
//    UNUSEDPARAM(entitiesList);
//    //CubeMesh cube = new CubeMesh(10.0f);
//    //cube.initialise();
//    //InstancedMesh cubeInstanceMesh = new InstancedMesh(cube);
//    //Handle cubeMeshHandle = new Handle("InstancedCube");
//    //ContentManager content = new ContentManager(game.Services, "content");
//    //Effect instancedCubeEffect = content.Load<Effect>("SimpleInstanceEffect");
//    //ModelManager.getInstance(game).addInstancedMesh(cubeMeshHandle, cubeInstanceMesh, instancedCubeEffect);
//    //Random randomGenerator = new Random(static_cast<size_t>(DateTime.Now.Ticks);
//    //for (uint entityCounter = 0; entityCounter < entitiesPerLevel; ++entityCounter)
//    //{
//    //    //Should loop over doors and switches if we generate this on a later stage so loop over the map and add these items
//    //    Entity entity = new Entity(game);
//    //    entity.Name = "Enemy_" + levelCounter.ToString() + "_" + entityCounter.ToString(); //Entity names are based on the level they are on followed by what the entity is
//    //    ++entityCounter;
//    //    AttributeHandleFloat enemyLife = new AttributeHandleFloat("life", 120.0f);
//    //    AttributeHandleInstanceModel enemyModel = new AttributeHandleInstanceModel("enemyCube", new InstanceModel("enemyCubeInstance" + entityCounter.ToString(), cubeMeshHandle));
//    //    entity.AddAttribute(enemyLife);
//    //    entity.AddAttribute(enemyModel);
//    //    entity.AddComponent("render_component");
//    //    entity.Active = true;
//    //    entitiesList.push_back(entity);
//    //    //Grab a room from the room list and generate a position within it
//    //    RoomNode room = (RoomNode)m_rooms[randomGenerator.Next(0, m_rooms.Count)];
//    //    Vector3 position = new Vector3(room.m_position.x * 50.0f + 25.0f, levelCounter * 50.0f + 5.0f, room.m_position.y * 50.0f - 25.0f);
//    //    DebugWrapper.Trace("Position of this entity is: {0}", position.ToString());
//    //    ((InstanceModel)enemyModel.getValue()).Position = position;
//    //}
//}
//
//}