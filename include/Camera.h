#ifndef CAMERA_H
#define CAMERA_H

#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum CAMERA_PROJECTION : unsigned short
{
    PERSPECTIVE,
    ORTHOGRAPHIC,
};

enum CAMERA_MOVEMENT : unsigned short
{
    BACKWARD,
    FORWARD,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    LEFT_ROTATION,  // Euler Angles
    RIGHT_ROTATION, // Euler Angles
    UP_ROTATION,    // Euler Angles
    DOWN_ROTATION,  // Euler Angles
};

class Camera
{
private:
    unsigned int m_VpWidth, m_VpHeight;

    CAMERA_PROJECTION m_Type;
    float m_NearPlane, m_FarPlane, m_FOV;

    // camera vectors
    glm::vec3 m_Pos, m_Up, m_Front;
    glm::mat4 m_Proj, m_View;

    bool cameraChanged = false;

public:
    Camera(unsigned int vpWidth, unsigned vpHeight, CAMERA_PROJECTION type, float nearPlane = 0.01f, float farPlane = 50.0f, float fov = 45.0f);
    ~Camera();

    // update viewport
    void SetViewport(unsigned int vpWidth, unsigned int vpHeight);

    // set Camera Uniforms
    void SetCameraAttr(Shader &shader);
    void SetViewProjMatrix(Shader &shader);

    // set Camera Vectors
    void SetPos(glm::vec3 position);
    void SetUp(glm::vec3 up);
    void SetFront(glm::vec3 front);

    // get Camera Matrices
    const glm::mat4 &GetProjectionMatrix() const { return m_Proj; }
    const glm::mat4 &GetViewMatrix() const { return m_View; }

    // get Camera Vectors
    inline glm::vec3 GetPos() const { return m_Pos; }
    inline glm::vec3 GetUp() const { return m_Up; }
    inline glm::vec3 GetFront() const { return m_Front; }

    // get Camera Attributes
    inline float GetFOV() { return m_FOV; }
    inline float GetNearPlane() { return m_NearPlane; }
    inline float GetFarPlane() { return m_FarPlane; }

private:
    void UpdateView();
    void UpdateProjection();
};

class FreeCameraController
{
private:
    Camera *m_MainCamera;

    float m_CameraSpeed = 1.5f, m_CameraRotationSpeed = 60.0f;

    float m_Yaw = -90.0f, m_Pitch = 0.0f; // roll omitted by now

    glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

public:
    FreeCameraController(Camera *camera);
    ~FreeCameraController(){};

    Camera *GetCamera() { return m_MainCamera; }

    void ResetCameraLocation();

    void ProcessKeyboardInput(CAMERA_MOVEMENT direction, float deltaTime);

    // camera vectors setters/getter
    void SetCameraPos(glm::vec3 position);
    void SetCameraUp(glm::vec3 up);
    void SetCameraFront(glm::vec3 front);

    glm::vec3 GetCameraPos() { return m_CameraPos; }
    glm::vec3 GetCameraUp() { return m_CameraUp; }
    glm::vec3 GetCameraFront() { return m_CameraFront; }

    // camera angles
    void SetYaw(float degree);
    void SetPitch(float degree);

    float GetYaw() { return m_Yaw; }
    float GetPitch() { return m_Pitch; }

private:
    void UpdateFront();
};

#endif