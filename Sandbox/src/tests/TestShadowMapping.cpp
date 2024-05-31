#include "pch.h"
#include "TestShadowMapping.h"
// #include "TestMenuLayer.h"
#include "imgui/imgui.h"

namespace OpenGLTest
{

    TestShadowMapping::TestShadowMapping()
    {
        m_Floor = Enxus::CreateRef<Enxus::Model>("Sandbox/res/models/models-ignore/map/map.obj");

        m_Box = Enxus::CreateRef<Enxus::Model>("Sandbox/res/models/box/box.obj");

        m_Shader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/advanced-lighting/shadow-mapping/shadow-map.vert",
                                                   "Sandbox/res/shaders/advanced-lighting/shadow-mapping/shadow-map.frag");

        m_ShadowMapDepthShader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/advanced-lighting/shadow-mapping/shadow-map-depth.vert",
                                                                 "Sandbox/res/shaders/advanced-lighting/shadow-mapping/shadow-map-depth.frag");

        m_ShadowMapFramebuffer = Enxus::CreateScope<ShadowMapFBO>(2048, 2048);

        //----------------- LIGHT MATRIX -------------------//
        m_LightPosition = glm::vec3(-2.0f, 4.0f, -1.0f);
        float nearPlane = 0.1f, farPlane = 20.5f;
        m_LightProjectionMatrix = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, nearPlane, farPlane);
    }

    TestShadowMapping::~TestShadowMapping()
    {
    }

    void TestShadowMapping::OnUpdate(Enxus::Camera &camera)
    {

        // Render scene as usual
        m_Shader->Bind();
        glm::mat4 model = glm::mat4(1.0f);
        m_Shader->SetMat4("uModel", model);
        m_Shader->SetMat4("uView", camera.GetViewMatrix());
        m_Shader->SetMat4("uProj", camera.GetProjectionMatrix());
        m_Shader->SetVec3("uViewPos", camera.GetPos());
        m_Shader->SetVec3("uLightPos", m_LightPosition);

        glm::mat4 lightView = glm::lookAt(m_LightPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

        m_LightSpaceMatrix = m_LightProjectionMatrix * lightView;

        m_Shader->SetMat4("uLightSpaceMatrix", m_LightSpaceMatrix);

        GLCall(glActiveTexture(GL_TEXTURE1));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_ShadowMapFramebuffer->GetDepthMapRendererId()));

        m_Shader->SetInt("uShadowDepthMap", 1);

        m_Shader->SetBool("uCastShadow", 1);
        {
            // Draw the floor
            Enxus::Renderer::DrawModel(m_Floor, m_Shader);
        }

        {
            // Draw Boxes
            for (int i = 0; i < 3; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((i - 1) * 2, 1.0, (i - 1) * 2));
                model = glm::scale(model, glm::vec3(0.3f));
                m_Shader->SetMat4("uModel", model);
                Enxus::Renderer::DrawModel(m_Box, m_Shader);
            }
        }
    }

    void TestShadowMapping::OnShadowPass()
    {
        m_ShadowMapFramebuffer->Bind();
        {
            m_ShadowMapDepthShader->Bind();
            glm::mat4 model = glm::mat4(1.0f);
            m_ShadowMapDepthShader->SetMat4("uModel", model);

            glm::mat4 lightView = glm::lookAt(m_LightPosition, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            m_LightSpaceMatrix = m_LightProjectionMatrix * lightView;
            m_ShadowMapDepthShader->SetMat4("uLightSpaceMatrix", m_LightSpaceMatrix);

            Enxus::Renderer::DrawModel(m_Floor, m_ShadowMapDepthShader);
            {
                // Draw Boxes
                for (int i = 0; i < 3; i++)
                {
                    glm::mat4 model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3((i - 1) * 2, -0.2f, (i - 1) * 2));
                    model = glm::scale(model, glm::vec3(0.3f));
                    m_ShadowMapDepthShader->SetMat4("uModel", model);
                    Enxus::Renderer::DrawModel(m_Box, m_ShadowMapDepthShader);
                }
            }
        }
        m_ShadowMapFramebuffer->Unbind();
    }

    void TestShadowMapping::OnImGuiRender()
    {
        ImGui::Begin("Shadow Map");
        ImVec2 size = ImGui::GetContentRegionAvail();
        intptr_t textureId = m_ShadowMapFramebuffer->GetDepthMapRendererId();
        ImGui::Image((void *)textureId, ImVec2(size.x, size.y));
        ImGui::End();

        ImGui::DragFloat3("Light Position", glm::value_ptr(m_LightPosition), 0.01f);
    }

}
