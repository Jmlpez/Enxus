#include "Camera.h"
// #include "glm/glm.hpp"
// #include "glm/gtc/matrix_transform.hpp"
// #include "glm/gtc/type_ptr.hpp"

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
    RecalculateProjection();
    RecalculateView();
}

Camera::~Camera()
{
}

void Camera::RecalculateView()
{
    m_View = glm::lookAt(m_Pos, m_Pos + m_Front, m_Up);
    cameraChanged = false;
}

void Camera::RecalculateProjection()
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

void Camera::SetViewport(unsigned int vpWidth, unsigned int vpHeight)
{
    // if nothing change just return
    if (m_VpWidth == vpWidth && m_VpHeight == vpHeight)
        return;

    m_VpWidth = vpWidth;
    m_VpHeight = vpHeight;
    RecalculateProjection();
}

void Camera::SetCameraAttr(Shader &shader)
{
    if (cameraChanged)
        RecalculateView();

    shader.SetMat4("u_Proj", m_Proj);
    shader.SetMat4("u_View", m_View);
    // set other attributes uniforms (e.g position, direction, etc)
}

void Camera::SetViewProjMatrix(Shader &shader)
{
    if (cameraChanged)
        RecalculateView();

    shader.SetMat4("u_Proj", m_Proj);
    shader.SetMat4("u_View", m_View);
}

void Camera::SetPos(glm::vec3 &position)
{
    m_Pos = position;
    cameraChanged = true;
}
void Camera::SetUp(glm::vec3 &up)
{
    m_Up = up;
    cameraChanged = true;
}
void Camera::SetFront(glm::vec3 &front)
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

void FreeCameraController::ProcessInput(GLFWwindow *window, float deltaTime)
{

    const float cameraSpeed = m_CameraSpeed * deltaTime;
    const float cameraRotationSpeed = m_CameraRotationSpeed * deltaTime;

    // std::cout << "Delta Time: " << miliseconds << " FPS: (" << 1.0f / deltaTime << ")" << std::endl;

    bool cameraMove = false;

    // camera locations
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        m_CameraPos += m_CameraFront * cameraSpeed;
        cameraMove = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        m_CameraPos -= m_CameraFront * cameraSpeed;
        cameraMove = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
        cameraMove = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * cameraSpeed;
        cameraMove = true;
    }
    if (cameraMove)
    {
        m_MainCamera->SetPos(m_CameraPos);
    }

    // camera rotations
    bool cameraRotate = false;
    //----------------- YAW ROTATIONS (around Y-axis) -------------------//

    if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
    {
        yaw -= cameraRotationSpeed;
        cameraRotate = true;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
    {
        yaw += cameraRotationSpeed;
        cameraRotate = true;
    }
    //----------------- PITCH ROTATIONS (around X-axis) -------------------//
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS)
    {
        pitch -= cameraRotationSpeed;
        cameraRotate = true;
    }
    if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
    {
        pitch += cameraRotationSpeed;
        cameraRotate = true;
    }

    if (!cameraRotate)
        return;
    glm::vec3 frontAux;
    frontAux.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    frontAux.y = sin(glm::radians(pitch));
    frontAux.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    m_CameraFront = glm::normalize(frontAux);
    m_MainCamera->SetFront(m_CameraFront);
}
