#ifndef CAMERA_H
#define CAMERA_H

#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Enxus
{
    enum class CameraProjection : unsigned short
    {
        PERSPECTIVE,
        ORTHOGRAPHIC,
    };

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
    private:
        unsigned int m_VpWidth, m_VpHeight;

        CameraProjection m_Type;
        float m_NearPlane, m_FarPlane, m_FOV;

        // camera vectors
        glm::vec3 m_Pos, m_Up, m_Front;
        glm::mat4 m_Proj, m_View;

        bool cameraChanged = false;

    public:
        Camera(unsigned int vpWidth, unsigned vpHeight, CameraProjection type, float nearPlane = 0.01f, float farPlane = 50.0f, float fov = 45.0f);
        ~Camera();

        // update viewport
        void OnResize(unsigned int vpWidth, unsigned int vpHeight);

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

}
#endif