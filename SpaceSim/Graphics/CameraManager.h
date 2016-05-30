#pragma once

#include "Camera.h"
#include <map>

class GameResource;

class CameraManager
{
public:
    CameraManager() {}    
    ~CameraManager() {}

    void update(float elapsedTime, double time);
    void update(float elapsedTime, double time, const Input& input);

    bool createCamera(const GameResource& resource, const std::string& name, const Vector3& position, const Vector3& lookAt, const Vector3& up);
    const Camera* getCamera(const std::string& name) const
    {
        std::map<std::string, Camera>::const_iterator it = m_cameras.find(name);
        return it != m_cameras.end() ? &it->second : 0;
    }

    typedef std::pair<std::string, Camera> CameraPair;
protected:
private:

    std::map<std::string, Camera> m_cameras;
};