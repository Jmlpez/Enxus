#include "FreeCameraController.h"

namespace Enxus
{

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

    void FreeCameraController::ProcessKeyboardInput(CameraMovement direction, float deltaTime)
    {

        const float cameraSpeed = m_CameraSpeed * deltaTime;
        const float cameraRotationSpeed = m_CameraRotationSpeed * deltaTime;

        // std::cout << "Delta Time: " << miliseconds << " FPS: (" << 1.0f / deltaTime << ")" << std::endl;

        bool cameraMove = false;

        // camera locations
        if (direction == CameraMovement::FORWARD)
        {
            m_CameraPos += m_CameraFront * cameraSpeed;
            cameraMove = true;
        }
        if (direction == CameraMovement::BACKWARD)
        {
            m_CameraPos -= m_CameraFront * cameraSpeed;
            cameraMove = true;
        }
        if (direction == CameraMovement::UP)
        {
            m_CameraPos += m_CameraUp * cameraSpeed;
            cameraMove = true;
        }
        if (direction == CameraMovement::DOWN)
        {
            m_CameraPos -= m_CameraUp * cameraSpeed;
            cameraMove = true;
        }
        if (direction == CameraMovement::LEFT)
        {
            m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
            cameraMove = true;
        }
        if (direction == CameraMovement::RIGHT)
        {
            m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
            cameraMove = true;
        }

        if (cameraMove)
            m_MainCamera->SetPos(m_CameraPos);

        // camera rotations

        //----------------- YAW ROTATIONS (around Y-axis) -------------------//
        bool cameraRotationKeyPress = false;
        if (direction == CameraMovement::LEFT_ROTATION)
        {
            m_Yaw -= cameraRotationSpeed;
            if (m_Yaw <= -360.0f)
                m_Yaw += 360.0f;
            cameraRotationKeyPress = true;
        }
        if (direction == CameraMovement::RIGHT_ROTATION)
        {
            m_Yaw += cameraRotationSpeed;
            if (m_Yaw >= 360.0f)
                m_Yaw -= 360.0f;
            cameraRotationKeyPress = true;
        }
        //----------------- PITCH ROTATIONS (around X-axis) -------------------//
        if (direction == CameraMovement::UP_ROTATION)
        {
            m_Pitch -= cameraRotationSpeed;
            cameraRotationKeyPress = true;
        }
        if (direction == CameraMovement::DOWN_ROTATION)
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