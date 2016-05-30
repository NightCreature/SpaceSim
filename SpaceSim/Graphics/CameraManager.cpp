#include "Graphics/CameraManager.h"
#include "Core/Settings/SettingsManager.h"
#include "Core/Resource/GameResource.h"

bool CameraManager::createCamera( const GameResource& resource, const std::string& name, const Vector3& position, const Vector3& lookAt, const Vector3& up )
{
    std::map<std::string, Camera>::iterator it = m_cameras.find(name);
    if (it == m_cameras.end() || m_cameras.empty())
    {
        const SettingsManager& sm = resource.getSettingsManager();
        const ISetting<float>* cameMoveSpeed = sm.getSetting<float>("CameraMovementSpeed");
        const ISetting<float>* cameRotationSpeed = sm.getSetting<float>("CameraRotationSpeed");
        Camera camera(cameMoveSpeed->getData(), cameRotationSpeed->getData());
        camera.positionCamera(position, lookAt, up);
        m_cameras.insert(CameraPair(name, camera));
        return true;
    }
    return false;
}

void CameraManager::update( float elapsedTime, double time )
{
    for (std::map<std::string, Camera>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
    {
        it->second.update(elapsedTime, time, DummyInput());
        it->second.createCamera();//Create the camera matrix
    }
}

void CameraManager::update( float elapsedTime, double time, const Input& input )
{
    for (std::map<std::string, Camera>::iterator it = m_cameras.begin(); it != m_cameras.end(); ++it)
    {
        it->second.update(elapsedTime, time, input);
        it->second.createCamera();//Create the camera matrix
    }
}
