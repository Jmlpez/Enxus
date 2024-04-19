#include "pch/pch.h"
#include "Camera.h"

namespace Enxus
{

    Camera::Camera(unsigned int vpWidth,
                   unsigned vpHeight,
                   CameraProjection type,
                   float nearPlane,
                   float farPlane,
                   float fov)
        : m_VpWidth(vpWidth),
          m_VpHeight(vpHeight),
          m_Type(type),
          m_NearPlane(nearPlane),
          m_FarPlane(farPlane),
          m_FOV(fov)
    {

        m_Pos = glm::vec3(0.0f, 0.0f, 3.0f);
        m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        m_Front = glm::vec3(0.0f, 0.0f, -1.0f);

        // in this case is the first calculation
        UpdateProjection();
        UpdateView();
    }

    Camera::~Camera()
    {
    }

    void Camera::UpdateView()
    {
        m_View = glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
        cameraChanged = false;
    }

    void Camera::UpdateProjection()
    {
        switch (m_Type)
        {
        case CameraProjection::PERSPECTIVE:
            m_Proj = glm::perspective(glm::radians(m_FOV), (float)m_VpWidth / (float)m_VpHeight, m_NearPlane, m_FarPlane);
            break;
        case CameraProjection::ORTHOGRAPHIC:
            m_Proj = glm::ortho(-m_VpWidth / 2.0f, m_VpWidth / 2.0f, -m_VpHeight / 2.0f, m_VpHeight / 2.0f, m_NearPlane, m_FarPlane);
            break;
        default:
            break;
        }
    }

    void Camera::OnResize(unsigned int vpWidth, unsigned int vpHeight)
    {
        // if nothing change just return
        if (m_VpWidth == vpWidth && m_VpHeight == vpHeight)
            return;

        m_VpWidth = vpWidth;
        m_VpHeight = vpHeight;
        UpdateProjection();
    }

    void Camera::SetCameraAttr(Shader &shader)
    {
        if (cameraChanged)
            UpdateView();

        shader.SetMat4("uProj", m_Proj);
        shader.SetMat4("uView", m_View);
        // set other attributes uniforms (e.g position, direction, etc)
    }

    void Camera::SetViewProjMatrix(Shader &shader)
    {
        if (cameraChanged)
            UpdateView();

        shader.SetMat4("uProj", m_Proj);
        shader.SetMat4("uView", m_View);
    }

    void Camera::SetPos(glm::vec3 position)
    {
        m_Pos = position;
        cameraChanged = true;
    }
    void Camera::SetUp(glm::vec3 up)
    {
        m_Up = up;
        cameraChanged = true;
    }
    void Camera::SetFront(glm::vec3 front)
    {
        m_Front = front;
        cameraChanged = true;
    }

}