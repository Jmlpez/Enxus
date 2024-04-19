#ifndef CAMERA_H
#define CAMERA_H

#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Enxus
{
    // Camara Projection will be Perspective

    enum class CameraMovement : unsigned short
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

    public:
        Camera(float fov, float aspectRatio, float nearPlane = 0.01f, float farPlane = 50.0f);
        ~Camera();

        // update viewport
        void SetViewportSize(unsigned int vpWidth, unsigned int vpHeight);

        // set Camera Vectors
        void SetPos(glm::vec3 position);
        void SetUp(glm::vec3 up);
        void SetFront(glm::vec3 front);

        // get Camera Matrices
        const glm::mat4 &GetProjectionMatrix() const { return m_Proj; }
        const glm::mat4 &GetViewMatrix() const { return m_View; }
        glm::mat4 GetViewProjectionMatrix() { return m_Proj * m_View; }

        // get Camera Vectors
        inline glm::vec3 GetPos() const { return m_Pos; }
        inline glm::vec3 GetUp() const { return m_Up; }
        inline glm::vec3 GetFront() const { return m_Front; }

        // get Camera Attributes
        inline float GetFOV() { return m_FOV; }
        inline float GetNearPlane() { return m_NearPlane; }
        inline float GetFarPlane() { return m_FarPlane; }

    private:
        void RecalculateView();
        void RecalculateProjection();

    private:
        unsigned int m_VpWidth, m_VpHeight;

        float m_FOV = 45.0f, m_AspectRatio = 0.0f, m_NearPlane = 0.01f, m_FarPlane = 50.0f;

        // camera vectors
        glm::vec3 m_Pos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);

        glm::mat4 m_Proj, m_View;
    };

}
#endif