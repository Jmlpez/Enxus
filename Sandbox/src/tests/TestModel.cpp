#include "pch.h"
#include "TestModel.h"
#include "imgui/imgui.h"

namespace OpenGLTest
{
    TestModel::TestModel() : m_LightDirection(-0.2f, -1.0f, -0.3f)
    {

        static const std::string s_modelsPathList[]{
            "Sandbox/res/models/models-ignore/trees/pine-tree-1.obj",
            "Sandbox/res/models/models-ignore/trees/pine-tree-2.obj",
            "Sandbox/res/models/models-ignore/trees/tree_1.obj",
            "Sandbox/res/models/models-ignore/trees/tree_2.obj",
            "Sandbox/res/models/models-ignore/trees/tree_3.obj",
            "Sandbox/res/models/models-ignore/trees/tree_4.obj",
            "Sandbox/res/models/models-ignore/trees/tree_5.obj",

        };
        int arrSize = sizeof(s_modelsPathList) / sizeof(s_modelsPathList[0]);
        for (int i = 0; i < arrSize; i++)
        {
            m_ModelsList.emplace_back(Enxus::CreateRef<Enxus::Model>(s_modelsPathList[i]));
        }

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

        GLCall(glEnable(GL_BLEND));
        // Blending equation: source_color * source_factor + destination_color * destination_factor
        // usually source factor is the alpha channel of the source_color
        // and destination factor 1 - the alpha channel of the source_color
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));
        GLCall(glFrontFace(GL_CW));
    }

    TestModel::~TestModel()
    {
    }

    void TestModel::OnUpdate(Enxus::Camera &camera)
    {
        Enxus::Renderer::ClearColor(0.13f, 0.14f, 0.16f, 1.0f);

        m_Shader->Bind();
        m_Shader->SetVec3("uCameraPos", camera.GetPos());
        m_Shader->SetVec3("uDirLight.direction", m_LightDirection);
        m_Shader->SetBool("uFlipsUVs", m_IsUVsFlipped);
        m_Shader->SetMat4("uProj", camera.GetProjectionMatrix());
        m_Shader->SetMat4("uView", camera.GetViewMatrix());

        m_Shader->Bind();
        int positionIndex = -(m_ModelsList.size() / 2);
        for (auto &model : m_ModelsList)
        {
            glm::mat4 mat = glm::translate(glm::mat4(1.0f), glm::vec3(positionIndex * 2.0f, 0.0f, 0.0f));
            m_Shader->SetMat4("uModel", mat);
            Enxus::Renderer::DrawModel(model, m_Shader);

            positionIndex++;
        }
    };

    void TestModel::OnImGuiRender()
    {
        ImGui::DragFloat3("Light direction", glm::value_ptr(m_LightDirection), 0.1f, -4.0f, 4.0f);
        ImGui::Checkbox("Flip UVs", &m_IsUVsFlipped);
    }

}
