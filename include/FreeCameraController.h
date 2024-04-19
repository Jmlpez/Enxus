#ifndef FREE_CAMERA_CONTROLLER_H
#define FREE_CAMERA_CONTROLLER_H

#include "Camera.h"

namespace Enxus
{

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

        void ProcessKeyboardInput(CameraMovement direction, float deltaTime);

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
}
#endif