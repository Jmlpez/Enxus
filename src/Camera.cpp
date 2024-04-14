#include "Camera.h"
#include <iostream>

namespace Enxus
{

    Camera::Camera(unsigned int vpWidth,
                   unsigned vpHeight,
                   CAMERA_PROJECTION type,
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
        case CAMERA_PROJECTION::PERSPECTIVE:
            m_Proj = glm::perspective(glm::radians(m_FOV), (float)m_VpWidth / (float)m_VpHeight, m_NearPlane, m_FarPlane);
            break;
        case CAMERA_PROJECTION::ORTHOGRAPHIC:
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

    //----------------- FREE CAMERA CONTROLLER -------------------//

    FreeCameraController::FreeCameraController(Camera *camera)
    {
        m_MainCamera = camera;
        m_MainCamera->SetPos(m_CameraPos);
        m_MainCamera->SetUp(m_CameraUp);
        m_MainCamera->SetFront(m_CameraFront);
    };

    void FreeCameraController::ResetCameraLocation()
    {
        m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

        m_MainCamera->SetPos(m_CameraPos);
        m_MainCamera->SetUp(m_CameraUp);
        m_MainCamera->SetFront(m_CameraFront);

        SetYaw(-90.0f);
        SetPitch(0.0f);
    }

    void FreeCameraController::UpdateFront()
    {
        glm::vec3 frontAux;
        frontAux.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        frontAux.y = sin(glm::radians(m_Pitch));
        frontAux.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

        m_CameraFront = glm::normalize(frontAux);
        m_MainCamera->SetFront(m_CameraFront);
    }

    void FreeCameraController::ProcessKeyboardInput(CAMERA_MOVEMENT direction, float deltaTime)
    {

        const float cameraSpeed = m_CameraSpeed * deltaTime;
        const float cameraRotationSpeed = m_CameraRotationSpeed * deltaTime;

        // std::cout << "Delta Time: " << miliseconds << " FPS: (" << 1.0f / deltaTime << ")" << std::endl;

        bool cameraMove = false;

        // camera locations
        if (direction == FORWARD)
        {
            m_CameraPos += m_CameraFront * cameraSpeed;
            cameraMove = true;
        }
        if (direction == BACKWARD)
        {
            m_CameraPos -= m_CameraFront * cameraSpeed;
            cameraMove = true;
        }
        if (direction == UP)
        {
            m_CameraPos += m_CameraUp * cameraSpeed;
            cameraMove = true;
        }
        if (direction == DOWN)
        {
            m_CameraPos -= m_CameraUp * cameraSpeed;
            cameraMove = true;
        }
        if (direction == LEFT)
        {
            m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
            cameraMove = true;
        }
        if (direction == RIGHT)
        {
            m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
            cameraMove = true;
        }

        if (cameraMove)
            m_MainCamera->SetPos(m_CameraPos);

        // camera rotations

        //----------------- YAW ROTATIONS (around Y-axis) -------------------//
        bool cameraRotationKeyPress = false;
        if (direction == LEFT_ROTATION)
        {
            m_Yaw -= cameraRotationSpeed;
            if (m_Yaw <= -360.0f)
                m_Yaw += 360.0f;
            cameraRotationKeyPress = true;
        }
        if (direction == RIGHT_ROTATION)
        {
            m_Yaw += cameraRotationSpeed;
            if (m_Yaw >= 360.0f)
                m_Yaw -= 360.0f;
            cameraRotationKeyPress = true;
        }
        //----------------- PITCH ROTATIONS (around X-axis) -------------------//
        if (direction == UP_ROTATION)
        {
            m_Pitch -= cameraRotationSpeed;
            cameraRotationKeyPress = true;
        }
        if (direction == DOWN_ROTATION)
        {
            m_Pitch += cameraRotationSpeed;
            cameraRotationKeyPress = true;
        }
        // to avoid flip (euler angles has this drawback)
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;

        if (!cameraRotationKeyPress)
            return;

        UpdateFront();
    }

    void FreeCameraController::SetCameraPos(glm::vec3 position)
    {
        m_CameraPos = position;
        m_MainCamera->SetPos(m_CameraPos);
    }
    void FreeCameraController::SetCameraUp(glm::vec3 up)
    {
        m_CameraUp = up;
        m_MainCamera->SetUp(m_CameraUp);
    }
    void FreeCameraController::SetCameraFront(glm::vec3 front)
    {
        m_CameraFront = front;
        m_MainCamera->SetFront(m_CameraFront);
    }

    void FreeCameraController::SetYaw(float degree)
    {
        m_Yaw = degree;
        UpdateFront();
    }
    void FreeCameraController::SetPitch(float degree)
    {
        m_Pitch = degree;
        UpdateFront();
    }

}