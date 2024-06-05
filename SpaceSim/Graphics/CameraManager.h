#pragma once

#include "Camera.h"
#include <map>

class Resource;

class CameraManager
{
public:
    CameraManager() {}    
    ~CameraManager() {}

    void update(float elapsedTime, double time);
    void update(float elapsedTime, double time, const Input& input);

    bool createCamera(const Resource& resource, const std::string& name, const Vector3& position, const Vector3& lookAt, const Vector3& up);
    const Camera* getCamera(const std::string& name) const
    {
        std::map<std::string, Camera>::const_iterator it = m_cameras.find(name);
        return it != m_cameras.end() ? &it->second : 0;
    }

    Camera* getCamera(const std::string& name)
    {
        std::map<std::string, Camera>::iterator it = m_cameras.find(name);
        return it != m_cameras.end() ? &it->second : 0;
    }

    std::vector<const Camera*> GetActiveCameras() const;

    typedef std::pair<std::string, Camera> CameraPair;
protected:
private:

    std::map<std::string, Camera> m_cameras;
};