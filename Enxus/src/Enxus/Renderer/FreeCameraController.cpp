#include "pch.h"
#include "FreeCameraController.h"
#include "Base.h"
#include "Input.h"
#include "Event.h"
#include "ApplicationEvent.h"
#include "KeyCodes.h"

namespace Enxus
{

    FreeCameraController::FreeCameraController(float aspectRatio)
        : m_Camera(45.0f, aspectRatio, 0.01f, 50.0f)
    {
        m_Camera.SetPos(m_CameraPos);
        m_Camera.SetUp(m_CameraUp);
        m_Camera.SetFront(m_CameraFront);
    };

    void FreeCameraController::ResetCameraLocation()
    {
        m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

        m_Camera.SetPos(m_CameraPos);
        m_Camera.SetUp(m_CameraUp);
        m_Camera.SetFront(m_CameraFront);

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
        m_Camera.SetFront(m_CameraFront);
    }

    void FreeCameraController::OnUpdate(Timestep ts)
    {
        float deltaTime = ts.GetSeconds();
        const float cameraSpeed = m_CameraTranslationSpeed * deltaTime;
        const float cameraRotationSpeed = m_CameraRotationSpeed * deltaTime;

        bool cameraMove = false;

        // camera locations
        if (Input::IsKeyPressed(Key::W))
        {
            m_CameraPos += m_CameraFront * cameraSpeed;
            cameraMove = true;
        }
        if (Input::IsKeyPressed(Key::S))
        {
            m_CameraPos -= m_CameraFront * cameraSpeed;
            cameraMove = true;
        }
        if (Input::IsKeyPressed(Key::Q))
        {
            m_CameraPos += m_CameraUp * cameraSpeed;
            cameraMove = true;
        }
        if (Input::IsKeyPressed(Key::E))
        {
            m_CameraPos -= m_CameraUp * cameraSpeed;
            cameraMove = true;
        }
        if (Input::IsKeyPressed(Key::A))
        {
            m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
            cameraMove = true;
        }
        if (Input::IsKeyPressed(Key::D))
        {
            m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
            cameraMove = true;
        }

        if (cameraMove)
            m_Camera.SetPos(m_CameraPos);

        // camera rotations

        //----------------- YAW ROTATIONS (around Y-axis) -------------------//
        bool cameraRotationKeyPress = false;
        if (Input::IsKeyPressed(Key::KP4))
        {
            m_Yaw -= cameraRotationSpeed;
            if (m_Yaw <= -360.0f)
                m_Yaw += 360.0f;
            cameraRotationKeyPress = true;
        }
        if (Input::IsKeyPressed(Key::KP6))
        {
            m_Yaw += cameraRotationSpeed;
            if (m_Yaw >= 360.0f)
                m_Yaw -= 360.0f;
            cameraRotationKeyPress = true;
        }
        //----------------- PITCH ROTATIONS (around X-axis) -------------------//
        if (Input::IsKeyPressed(Key::KP8))
        {
            m_Pitch -= cameraRotationSpeed;
            cameraRotationKeyPress = true;
        }
        if (Input::IsKeyPressed(Key::KP2))
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
    void FreeCameraController::OnEvent(Event &event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(FreeCameraController::OnWindowResized));
    }

    void FreeCameraController::OnResize(unsigned int width, unsigned int height)
    {
        m_Camera.SetViewportSize(width, height);
    }

    bool FreeCameraController::OnWindowResized(WindowResizeEvent &event)
    {
        OnResize((float)event.GetWidth(), (float)event.GetHeight());
        return true;
    }

    void FreeCameraController::SetCameraPos(glm::vec3 position)
    {
        m_CameraPos = position;
        m_Camera.SetPos(m_CameraPos);
    }
    void FreeCameraController::SetCameraUp(glm::vec3 up)
    {
        m_CameraUp = up;
        m_Camera.SetUp(m_CameraUp);
    }
    void FreeCameraController::SetCameraFront(glm::vec3 front)
    {
        m_CameraFront = front;
        m_Camera.SetFront(m_CameraFront);
    }
    // camera angles
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