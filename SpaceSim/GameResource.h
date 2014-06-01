#pragma once
#include "resourceable.h"

class CameraManager;
class DeviceManager;
class SettingsManager;
class TextureManager;
class GameObjectManager;
class ModelManager;
class ParticleSystemManager;
class LightManager;
class LaserManager;
class ShaderCache;
class EffectCache;
class Paths;

class GameResource : public Resource
{
public:
    GameResource(ResourceableParam(CameraManager, cameraManager), ResourceableParam(DeviceManager, deviceManager)
            ,ResourceableParam(SettingsManager, settingsManager), ResourceableParam(TextureManager, textureManager)
            ,ResourceableParam(GameObjectManager, gameObjectManager), ResourceableParam(ModelManager, modelManager)
            ,ResourceableParam(ParticleSystemManager, pfxManager), ResourceableParam(LightManager, lightManager)
            ,ResourceableParam(LaserManager, laserManager), ResourceableParam(ShaderCache, shaderCache)
            , ResourceableParam(EffectCache, effectCache), ResourceableParam(Paths, paths)) :
            ResourceInitialiser(CameraManager, cameraManager),
            ResourceInitialiser(DeviceManager, deviceManager),
            ResourceInitialiser(SettingsManager, settingsManager),
            ResourceInitialiser(TextureManager, textureManager),
            ResourceInitialiser(GameObjectManager, gameObjectManager),
            ResourceInitialiser(ModelManager, modelManager),
            ResourceInitialiser(ParticleSystemManager, pfxManager),
            ResourceInitialiser(LightManager, lightManager),
            ResourceInitialiser(LaserManager, laserManager),
            ResourceInitialiser(ShaderCache, shaderCache),
            ResourceInitialiser(EffectCache, effectCache),
            ResourceInitialiser(Paths, paths)
    {}
    ~GameResource() {}

    ResourceDefinition(CameraManager)
    ResourceDefinition(DeviceManager)
    ResourceDefinition(SettingsManager)
    ResourceDefinition(TextureManager)
    ResourceDefinition(GameObjectManager)
    ResourceDefinition(ModelManager)
    ResourceDefinition(ParticleSystemManager)
    ResourceDefinition(LightManager)
    ResourceDefinition(LaserManager)
    ResourceDefinition(ShaderCache)
    ResourceDefinition(EffectCache)
    ResourceDefinition(Paths);
};

//This class should contain all the accessors for the game resource, means a resource can have everthing in a list and this class is the one objects use to interfact with the systems
class GameResourceHelper
{
public:
    GameResourceHelper(Resource* resource) : m_resource(resource) {}
    GameResourceHelper(const Resource* resource) : m_resource((Resource*)resource) {}
    ~GameResourceHelper() {}

    const GameResource& getGameResource() const { return *(GameResource*)m_resource; }
    GameResource* getWritableGameResource() const { return (GameResource*)m_resource; }
private:
    Resource* m_resource;
};
