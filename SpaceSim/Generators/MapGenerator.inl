//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//template<size_t x, size_t y, size_t z>
//void MapGenerator<x, y, z>::Initialise(size_t numberOfThreadsToSpawn, const Vector4s& roomDimensionsRange, int roomsPerFloor, size_t connectingCooridorsPerFloor, size_t connectingCorridorsBetweenFloors)
//{
//    m_numberOfThreadsToSpawn = numberOfThreadsToSpawn;
//
//    //fix resize
//    //m_map.reserve(static_cast<size_t>(m_mapDimensions.x()), static_cast<size_t>(m_mapDimensions.y()), static_cast<size_t>(m_mapDimensions.z()));
//    //m_collisionMap.reserve(static_cast<size_t>(m_mapDimensions.x()), static_cast<size_t>(m_mapDimensions.y()), static_cast<size_t>(m_mapDimensions.z()));
//    //m_map.Initialize();
//    m_roomDimensionRange = roomDimensionsRange;
//    m_roomsPerFloor = roomsPerFloor;
//    m_connectingCorridorsPerFloor = connectingCooridorsPerFloor;
//    m_connectingCorridorsBetweenFloors = connectingCorridorsBetweenFloors;
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//template<size_t x, size_t y, size_t z>
//inline bool MapGenerator<x, y, z>::generateMap(Resource* resource)
//{
//    //Needs optimising as this takes about 1.5 - 2.0 seconds on a 50x50x50 map
//    //This needs to kick off threads which handle 4 to 5 levels each
//    size_t numberOfLevelsToHandlePerThread = 0;
//    size_t numberOfThreadsToSpawn = m_numberOfThreadsToSpawn;
//    if (m_numberOfThreadsToSpawn > y)
//    {
//        numberOfThreadsToSpawn = y;
//        numberOfLevelsToHandlePerThread = 1; //Not so effiecient but will make the algorithm work in all cases
//    }
//    else
//    {
//        numberOfLevelsToHandlePerThread = y / m_numberOfThreadsToSpawn; //Only spawn 4 threads on a quad core machine, decimal point is ignored
//        ++numberOfLevelsToHandlePerThread;
//    }
//    //This should be jobs
//    //Thread[] threadGenerators = new Thread[numberOfThreadsToSpawn];
//    //FloorGenerationParameters[] parameters = new FloorGenerationParameters[numberOfThreadsToSpawn];
//    GameResourceHelper helper = GameResourceHelper(resource);
//    auto& jobQueue = helper.getWriteableResource().getJobQueue();
//    for (int counter = 0; counter < numberOfThreadsToSpawn; ++counter)
//    {
//        FloorGenerationParameters<x, y, z> parameter;
//        parameter.m_floorDimensions[0] = x;
//        parameter.m_floorDimensions[1] = y;
//        parameter.m_floorDimensions[2] = z;
//        parameter.m_roomDimensionRange = m_roomDimensionRange;
//        parameter.m_map = &m_map;
//        parameter.m_startFloor = counter * numberOfLevelsToHandlePerThread;
//        parameter.m_endFloor = math::clamp(parameter.m_startFloor + numberOfLevelsToHandlePerThread, 0ull, y);
//        parameter.m_roomsPerFloor = m_roomsPerFloor;
//        parameter.m_entitiesPerFloor = m_roomsPerFloor;
//        parameter.m_runningJobs = &m_runningJobs;
//        parameter.m_connectingCorridorsPerFloor = m_connectingCorridorsPerFloor;
//        m_jobs.emplace_back(MapLayerJob(parameter));
//    }
//
//    for (size_t counter = 0; counter < m_jobs.size(); ++counter)
//    {
//        jobQueue.AddJob(&(m_jobs[counter]));
//    }
//
//    m_runningJobs = m_jobs.size();
//
//    auto model = generateGeometry(); //probably want to return this
//    //CreatedModel already does this I think, we need to do something here
//    //generateCollisionMeshesForLevel(mapEntity); //push_back the map to the entity
//    //model.Value().createModel();
//
//    //#if DEBUG
//    //writeMap();
//    //#endif
//                //This adds the model to the render system so it can be rendered
//    //mapEntity.AddComponent("render_component");
//    //GameObjectManager.getInstance(game).AddGameObject(mapEntity);
//    //mapEntity.Active = true;
//    timestamp = PerformanceTimer().getTimeStamp();
//    return true;
//}
//
//static bool printResult = false;
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------.
//template<size_t x, size_t y, size_t z>
//inline void MapGenerator<x, y, z>::Update()
//{
//    if (m_runningJobs > 0)
//    {
//        return;
//    }
//    else if (!printResult)
//    {
//        PerformanceTimer timer;
//        auto timestamp2 = timer.getTimeStamp();
//
//        MSG_TRACE_CHANNEL("[MAPGENERATOR]", "FINISHED JOBS in %f", (timestamp2 - timestamp) / timer.getResolution());
//        printResult = true;
//    }
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//template<size_t x, size_t y, size_t z>
//CreatedModel MapGenerator<x, y, z>::generateGeometry()
//{
//    //size_t maxWidth = m_mapDimensions.Get(0);
//    //size_t maxSlice = maxWidth * m_mapDimensions.Get(1);
//
//    CreatedModel model;
//
//    ////Create materials for floor, ceiling and walls
//    //Material floorMaterial;
//    //floorMaterial.setEffectHash(hashString("ShaderNameToUse.fx"));
//    //floorMaterial.setDiffuse(Color::yellow());
//    //floorMaterial.setAmbient(Color(0.2f, 0.2f, 0.2f));
//    //floorMaterial.setShininess(25000);
//    //floorMaterial.addTextureReference(Material::TextureSlotMapping(hashString("floorTexture.png"), Material::TextureSlotMapping::Diffuse0));
//    ////map.floorMaterial = floorMaterial;
//
//    //Material ceilingMaterial;
//    //ceilingMaterial.setEffectHash(hashString("ShaderNameToUse.fx"));
//    //ceilingMaterial.setDiffuse(Color::white());
//    //ceilingMaterial.setAmbient(Color(0.2f, 0.2f, 0.2f));
//    //ceilingMaterial.setShininess(25000);
//    //ceilingMaterial.addTextureReference(Material::TextureSlotMapping(hashString("floorTexture.png"), Material::TextureSlotMapping::Diffuse0));
//    ////map.ceilingMaterial = ceilingMaterial;
//
//    //Material wallMaterial;
//    //wallMaterial.setEffectHash(hashString("ShaderNameToUse.fx"));
//    //wallMaterial.setDiffuse(Color::darkgray());
//    //wallMaterial.setAmbient(Color(0.2f, 0.2f, 0.2f));
//    //wallMaterial.setShininess(25000);
//    //wallMaterial.addTextureReference(Material::TextureSlotMapping(hashString("floorTexture.png"), Material::TextureSlotMapping::Diffuse0));
//    ////map.wallMaterial = wallMaterial;
//
//    //auto wallSize = Vector2(50.0f, 50.0f);
//    //for (int yCounter = 0; yCounter < m_mapDimensions.Get(1); ++yCounter)
//    //{
//    //    for (int xCounter = 0; xCounter < m_mapDimensions.Get(0); ++xCounter)
//    //    {
//    //        for (int zCounter = 0; zCounter < m_mapDimensions.Get(2); ++zCounter)
//    //        {
//    //            //This also generates the collision node map, which at the end needs to be culled of empty nodes!
//    //            if (m_map[xCounter, yCounter, zCounter] == RoomType::eRoom)
//    //            {
//    //                auto translationVector = Vector3(50.0f * xCounter, 50.0f * yCounter, 50.0f * zCounter);
//    //                Matrix44 transform;
//    //                transform.setTranslate(translationVector);//Ceiling level top left corner of room
//
//    //                if ((yCounter >= 1 && m_map[xCounter, yCounter - 1, zCounter] != RoomType::eRoom) || yCounter == 0) //Left of current room is no room or is edge of map so generate a wall
//    //                {
//    //                    auto ceiling = Plane(wallSize, Vector3::yAxis());//50x50 at 0,0,0
//    //                    ceiling.initialise();
//
//    //                    //Might just do this or create a different game object that does this in the end from the information we store here
//    //                    //Face::CreationParams params;
//    //                    ////    params.resource = m_resource;
//    //                    ////    params.shaderInstance = &shaderInstance;
//    //                    //params.nrVerticesInX = m_rows;
//    //                    //params.nrVerticesInY = m_coloms;
//    //                    //params.width = m_widthendpos - m_widthstartpos;
//    //                    //params.height = m_heightendpos - m_heightstartpos;
//    //                    //params.fillx = m_fillx;
//    //                    //params.filly = m_filly;
//    //                    //params.fillz = m_fillz;
//    //                    //params.fillvalue = m_fillvalue;
//    //                    //params.invertNormal = m_invertNormal;
//    //                    //params.changeWindingOrder = m_changeWindingOrder;
//    //                    //params.m_materialParameters = m_materialParameters;
//    //                    ////if (10 != m_rows && 10 != m_coloms)
//    //                    ////{
//    //                    ////    params.tesselate = false;
//    //                    ////}
//    //                    ////else
//    //                    //{
//    //                    //    params.tesselate = true;
//    //                    //}
//
//    //                    //auto resource = GameResourceHelper(m_resource).getWriteableResource();
//    //                    //DECLAREANDCREATERESOURCEMESSAGE(createPlaneModel, Face::CreationParams);
//    //                    //createPlaneModel.SetData(params);
//    //                    //createPlaneModel.SetGameObjectId(static_cast<size_t>(m_nameHash)); //Not super but should work for now
//
//    //                    //resource.m_messageQueues->getUpdateMessageQueue()->addMessage(createPlaneModel);
//    //                    //map.AddXZPlane(ceiling, transform);
//    //                    //CollisionNode node = CollisionNode();
//    //                    //node.setWall(CollisionNode.WallType.CEILING, true);
//    //                    //m_collisionMap[xCounter, yCounter, zCounter] = node;
//    //                }
//    //                if ((yCounter < m_mapDimensions.Get(1) - 1 && m_map[xCounter, yCounter + 1, zCounter] != RoomType::eRoom) || m_mapDimensions.Get(1) - 1 == yCounter) //Right of current room is no room or is edge of map so generate a wall
//    //                {
//    //                    translationVector.Get(1) += 50.0f;
//    //                    transform.setTranslate(translationVector);
//    //                    auto floor = Plane(wallSize, -Vector3::yAxis());//50x50 at 0,0,0
//    //                    floor.initialise();
//    //                    map.AddXZPlane(floor, transform);
//    //                    //CollisionNode node = m_collisionMap[xCounter, yCounter, zCounter];
//    //                    //if (m_collisionMap[xCounter, yCounter, zCounter] == null)
//    //                    //{
//    //                    //    node = CollisionNode();
//    //                    //}
//    //                    //node.setWall(CollisionNode.WallType.FLOOR, true);
//    //                    //m_collisionMap[xCounter, yCounter, zCounter] = node;
//    //                }
//    //                translationVector.Get(1) = 50.0f * yCounter; //Reset after we have droped the floor down
//
//    //                if ((xCounter >= 1 && m_map[xCounter - 1, yCounter, zCounter] != RoomType::eRoom) || xCounter == 0) //Left of current room is no room or is edge of map so generate a wall
//    //                {
//    //                    //.Get(2) plane wall to generate
//    //                    auto leftWall = Plane(wallSize, Vector3::xAxis());
//    //                    leftWall.initialise();
//    //                    map.AddYZPlane(leftWall, Matrix.CreateTranslation(translationVector));
//    //                    //CollisionNode node = m_collisionMap[xCounter, yCounter, zCounter];
//    //                    //if (m_collisionMap[xCounter, yCounter, zCounter] == null)
//    //                    //{
//    //                    //    node = CollisionNode();
//    //                    //}
//    //                    //node.setWall(CollisionNode.WallType.LEFTWALL, true);
//    //                    //m_collisionMap[xCounter, yCounter, zCounter] = node;
//    //                }
//    //                if ((xCounter < m_mapDimensions.Get(0) - 1 && m_map[xCounter + 1, yCounter, zCounter] != RoomType::eRoom) || m_mapDimensions.Get(0) - 1 == xCounter) //Right of current room is no room or is edge of map so generate a wall
//    //                {
//    //                    //.Get(2) plane wall to generate
//    //                    auto leftWall = Plane(wallSize, -Vector3::xAxis());
//    //                    leftWall.initialise();
//    //                    translationVector.Get(0) += 50.0f;
//    //                    map.AddYZPlane(leftWall, Matrix.CreateTranslation(translationVector));
//    //                    translationVector.Get(0) -= 50.0f;
//    //                    //CollisionNode node = m_collisionMap[xCounter, yCounter, zCounter];
//    //                    //if (m_collisionMap[xCounter, yCounter, zCounter] == null)
//    //                    //{
//    //                    //    node = CollisionNode();
//    //                    //}
//    //                    //node.setWall(CollisionNode.WallType.RIGHTWALL, true);
//    //                    //m_collisionMap[xCounter, yCounter, zCounter] = node;
//    //                }
//
//    //                if ((zCounter >= 1 && m_map[xCounter, yCounter, zCounter - 1] != RoomType::eRoom) || zCounter == 0) //Back of current room is no room or is edge of map so generate a wall
//    //                {
//    //                    //.Get(2) plane wall to generate
//    //                    auto leftWall = Plane(wallSize, Vector3::zAxis());
//    //                    leftWall.initialise();
//    //                    translationVector.Get(2) -= 50.0f;
//    //                    map.AddXYPlane(leftWall, Matrix.CreateTranslation(translationVector));
//    //                    translationVector.Get(2) += 50.0f;
//    //                    //CollisionNode node = m_collisionMap[xCounter, yCounter, zCounter];
//    //                    //if (m_collisionMap[xCounter, yCounter, zCounter] == null)
//    //                    //{
//    //                    //    node = CollisionNode();
//    //                    //}
//    //                    //node.setWall(CollisionNode.WallType.BACKWALL, true);
//    //                    //m_collisionMap[xCounter, yCounter, zCounter] = node;
//    //                }
//    //                if ((zCounter < m_mapDimensions.Get(2) - 1 && m_map[xCounter, yCounter, zCounter + 1] != RoomType::eRoom) || m_mapDimensions.Get(0) - 1 == zCounter) //Front of current room is no room or is edge of map so generate a wall
//    //                {
//    //                    //.Get(2) plane wall to generate
//    //                    auto leftWall = Plane(wallSize, -Vector3::zAxis());
//    //                    leftWall.initialise();
//    //                    map.AddXYPlane(leftWall, Matrix.CreateTranslation(translationVector));
//    //                    //CollisionNode node = m_collisionMap[xCounter, yCounter, zCounter];
//    //                    //if (m_collisionMap[xCounter, yCounter, zCounter] == null)
//    //                    //{
//    //                    //    node = CollisionNode();
//    //                    //}
//    //                    //node.setWall(CollisionNode.WallType.FRONTWALL, true);
//    //                    //m_collisionMap[xCounter, yCounter, zCounter] = node;
//    //                }
//    //            }
//    //        }
//    //    }
//    //}
//
//    return model;
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
////void MapGenerator::generateCollisionMeshesForLevel(Entity& entity)
////{
////    //Implement level by level generation of a collision mesh
////    AttributeHandleCollisionMap collisionMap = new AttributeHandleCollisionMap("collision_mesh", m_collisionMap);
////    entity.AddAttribute(collisionMap);
////    ComponentManager cm = ComponentManager.getInstance(game);
////    entity.AddComponent("map_collision_component");
////}
