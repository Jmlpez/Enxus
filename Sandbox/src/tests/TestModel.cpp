#include "pch.h"
#include "TestModel.h"
#include "imgui/imgui.h"

namespace OpenGLTest
{
    TestModel::TestModel() : m_LightDirection(-0.2f, -1.0f, -0.3f)
    {
        m_Backpack = Enxus::CreateRef<Enxus::Model>("Sandbox/res/models/backpack/backpack.obj");
        m_Box = Enxus::CreateRef<Enxus::Model>("Sandbox/res/models/box/box.obj");

        m_Shader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/model/backpack.vert", "Sandbox/res/shaders/model/backpack.frag");

        m_Shader->Bind();
        //----------------- Backpack position -------------------//
        glm::mat4 model = glm::mat4(1.0f);
        m_Shader->SetMat4("uModel", model);

        // for specular lighting
        m_Shader->SetFloat("material.shininess", 32.0f);

        //----------------- DIRECTIONAL LIGHTING -------------------//

        m_Shader->SetVec3("uDirLight.direction", m_LightDirection);
        m_Shader->SetFloat3("uDirLight.ambient", 0.1f, 0.1f, 0.1f);
        m_Shader->SetFloat3("uDirLight.diffuse", 1.0f, 1.0f, 1.0f);
        m_Shader->SetFloat3("uDirLight.specular", 1.0f, 1.0f, 1.0f);
    }

    TestModel::~TestModel()
    {
    }

    void TestModel::OnUpdate(Enxus::Camera &camera)
    {
        m_Shader->Bind();
        m_Shader->SetVec3("uCameraPos", camera.GetPos());
        m_Shader->SetVec3("uDirLight.direction", m_LightDirection);
        m_Shader->SetBool("uFlipsUVs", m_IsUVsFlipped);
        m_Shader->SetMat4("uProj", camera.GetProjectionMatrix());
        m_Shader->SetMat4("uView", camera.GetViewMatrix());

        m_Shader->Unbind();

        Enxus::Renderer::ClearColor(0.13f, 0.14f, 0.16f, 1.0f);

        m_Shader->Bind();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 0.0f));
        m_Shader->SetMat4("uModel", model);
        Enxus::Renderer::DrawModel(m_Backpack, m_Shader);
        //----------------- Box position -------------------//

        model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, -1.0f));
        m_Shader->SetMat4("uModel", model);
        Enxus::Renderer::DrawModel(m_Box, m_Shader);
    };

    void TestModel::OnImGuiRender()
    {
        ImGui::DragFloat3("Light direction", glm::value_ptr(m_LightDirection), 0.1f, -4.0f, 4.0f);
        ImGui::Checkbox("Flip UVs", &m_IsUVsFlipped);
    }

}
