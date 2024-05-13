#include "pch.h"
#include "Camera.h"

namespace Enxus
{

    Camera::Camera(
        float fov,
        float aspectRatio,
        float nearPlane,
        float farPlane)
        : m_FOV(fov),
          m_AspectRatio(aspectRatio),
          m_NearPlane(nearPlane),
          m_FarPlane(farPlane)
    {

        // in this case is the first calculation
        RecalculateView();
        RecalculateProjection();
    }

    Camera::~Camera()
    {
    }

    void Camera::RecalculateView()
    {
        m_View = glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
    }

    void Camera::RecalculateProjection()
    {
        m_Proj = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearPlane, m_FarPlane);
    }

    void Camera::SetViewportSize(unsigned int vpWidth, unsigned int vpHeight)
    {
        // if nothing change just return
        if (m_VpWidth == vpWidth && m_VpHeight == vpHeight)
            return;

        m_VpWidth = vpWidth;
        m_VpHeight = vpHeight;
        m_AspectRatio = (float)m_VpWidth / (float)m_VpHeight;
        RecalculateProjection();
    }

    void Camera::SetPos(glm::vec3 position)
    {
        m_Pos = position;
        RecalculateView();
    }
    void Camera::SetUp(glm::vec3 up)
    {
        m_Up = up;
        RecalculateView();
    }
    void Camera::SetFront(glm::vec3 front)
    {
        m_Front = front;
        RecalculateView();
    }

    void Camera::SetFOV(float fov)
    {
        m_FOV = fov;
        RecalculateProjection();
    }
    void Camera::SetNearPlane(float nearPlane)
    {
        m_NearPlane = nearPlane;
        RecalculateProjection();
    }
    void Camera::SetFarPlane(float farPlane)
    {
        m_FarPlane = farPlane;
        RecalculateProjection();
    }

}