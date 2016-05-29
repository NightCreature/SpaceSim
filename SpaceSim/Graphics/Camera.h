#pragma once

#include "Input/Input.h"
#include "Math/matrix44.h"
#include "Math/matrixmath.h"
#include "Math/vector3.h"

#include <sstream>
#include <iostream>

class Camera
{
public:
    Camera();
    Camera(float movementSpeed, float rotationSpeed);
    ~Camera();

    void update(float elapsedTime, double time, const Input& input );

    ///Creates the camera Matrix in DX style
    Matrix44 createCamera();

    void positionCamera(const Vector3& eye, const Vector3& lookat, const Vector3& up);
    void roll(float angle);
    void pitch(float angle);
    void yaw(float angle);
    void moveOverPlane(float deltau, float deltav, float deltan);

    const Vector3 getEye() const {return m_eye;}
    const Vector3 getLookAt() const {return m_cameraForward;}
    const Vector3 getUp() const {return m_cameraUp;}
    const Vector3 getRight() const {return m_cameraRight;}
    float getRoll() const {return m_roll;}
    float getPitch() const {return m_pitch;}
    float getYaw() const {return m_yaw;}
    Matrix44 getCamera() const {return m_camera;}
    Matrix44 getInvCamera() const {return m_invcamera;}
    void setEye(const Vector3& eye) {m_eye = eye;}

    void moveAlongDirection(const float distance)
    {
        m_eye += m_cameraForward * distance;
    }

    void strafe(const float amount)
    {
        m_eye += -amount * m_cameraRight;//Needed to invert the input amount to correct for left right
    }
protected:
private:
    float restrictAngleTo360(float angle) const;
    Matrix44 m_camera;
    Matrix44 m_invcamera;

    Vector3 m_eye; ///position of the eye in the world
    Vector3 m_cameraPositionDelta;
    Vector3 m_cameraRight;
    Vector3 m_cameraUp;
    Vector3 m_cameraForward;
    //Vector4 m_right; ///right axis of the camera

    float m_roll; ///angle to rotate about in the z plane
    float m_yaw; ///angle to rotate about in the y plane
    float m_pitch; ///angle to rotate about in the x plane

    float m_movementSpeed;
    float m_rotationSpeed;
    static const float m_maxPitchAngle;
};
