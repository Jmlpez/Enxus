#include "pch.h"
#include "TestBlinnPhong.h"
#include "imgui/imgui.h"

namespace OpenGLTest
{
    TestBlinnPhong::TestBlinnPhong()
    {
        //----------------- PLANE -------------------//
        std::vector<Enxus::VertexData> vertices{
            {glm::vec3(5.0f, -0.5f, 5.0f), glm::vec2(2.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-5.0f, -0.5f, 5.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-5.0f, -0.5f, -5.0f), glm::vec2(0.0f, 2.0f), glm::vec3(0.0f)},
            {glm::vec3(5.0f, -0.5f, 5.0f), glm::vec2(2.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-5.0f, -0.5f, -5.0f), glm::vec2(0.0f, 2.0f), glm::vec3(0.0f)},
            {glm::vec3(5.0f, -0.5f, -5.0f), glm::vec2(2.0f, 2.0), glm::vec3(0.0f)},
        };
        std::vector<Enxus::TextureMesh2DData> texturesFloor{
            {"Sandbox/res/images/wood.png", Enxus::TextureType::DIFFUSE},
        };
        m_Floor = Enxus::CreateRef<Enxus::Mesh>(vertices, texturesFloor);

        m_Shader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/advanced-lighting/blinn-phong.vert",
                                                   "Sandbox/res/shaders/advanced-lighting/blinn-phong.frag");

        m_UseBlinnPhong = false;
        m_LightPosition = glm::vec3(0.0f);
    }
    TestBlinnPhong::~TestBlinnPhong()
    {
    }

    void TestBlinnPhong::OnUpdate(Enxus::Camera &camera)
    {

        m_Shader->Bind();
        glm::mat4 model = glm::mat4(1.0f);
        m_Shader->SetMat4("uModel", model);
        m_Shader->SetMat4("uProj", camera.GetProjectionMatrix());
        m_Shader->SetMat4("uView", camera.GetViewMatrix());

        m_Shader->SetBool("uBlinn", m_UseBlinnPhong);
        m_Shader->SetVec3("uViewPos", camera.GetPos());
        m_Shader->SetVec3("uLightPos", m_LightPosition);

        Enxus::Renderer::DrawMesh(m_Floor, m_Shader);
    }
    void TestBlinnPhong::OnImGuiRender()
    {
        ImGui::Checkbox("Use Blinn-Phong", &m_UseBlinnPhong);
        ImGui::DragFloat3("Light Position", glm::value_ptr(m_LightPosition), 0.01f);
    }

}
