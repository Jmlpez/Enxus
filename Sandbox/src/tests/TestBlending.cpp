#include "pch.h"
#include "TestBlending.h"
#include "imgui/imgui.h"

namespace OpenGLTest
{
    TestBlending::TestBlending()
        : m_TransparentObjsPositions{
              glm::vec3(-1.5f, 0.0f, -0.48f),
              glm::vec3(1.5f, 0.0f, 0.51f),
              glm::vec3(0.0f, 0.0f, 0.7f),
              glm::vec3(-0.3f, 0.0f, -2.3f),
              glm::vec3(0.5f, 0.0f, -0.6f)}
    {
        //----------------- BOX -------------------//

        std::vector<Enxus::VertexData> vertices{
            {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0), glm::vec3(0.0f)},
        };

        std::vector<Enxus::TextureData2D> textures{
            {"Sandbox/res/images/marble.jpg", Enxus::TextureType::DIFFUSE},
        };

        m_Box = Enxus::CreateRef<Enxus::Mesh>(vertices, textures);
        //----------------- PLANE -------------------//
        vertices = std::vector<Enxus::VertexData>{
            {glm::vec3(5.0f, -0.5f, 5.0f), glm::vec2(2.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-5.0f, -0.5f, 5.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-5.0f, -0.5f, -5.0f), glm::vec2(0.0f, 2.0f), glm::vec3(0.0f)},
            {glm::vec3(5.0f, -0.5f, 5.0f), glm::vec2(2.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(-5.0f, -0.5f, -5.0f), glm::vec2(0.0f, 2.0f), glm::vec3(0.0f)},
            {glm::vec3(5.0f, -0.5f, -5.0f), glm::vec2(2.0f, 2.0), glm::vec3(0.0f)},
        };
        std::vector<Enxus::TextureData2D> texturesFloor{
            {"Sandbox/res/images/metal.png", Enxus::TextureType::DIFFUSE},
        };
        m_Floor = Enxus::CreateRef<Enxus::Mesh>(vertices, texturesFloor);

        //----------------- GRASS -------------------//
        std::vector<Enxus::VertexData> grassVertices{
            {glm::vec3(0.0f, 0.5f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(0.0f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(1.0f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(0.0f, 0.5f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f)},
            {glm::vec3(1.0f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f)},
            {glm::vec3(1.0f, 0.5f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f)},
        };

        std::vector<Enxus::TextureData2D> grassTextures{{
            "Sandbox/res/images/window.png",
            Enxus::TextureType::DIFFUSE,
        }};

        m_TransparentObj = Enxus::CreateRef<Enxus::Mesh>(grassVertices, grassTextures);
        // Create an API to set TexParameter
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        m_Shader = Enxus::CreateRef<Enxus::Shader>("Sandbox/res/shaders/advanced-opengl/blending/basic.vert",
                                                   "Sandbox/res/shaders/advanced-opengl/blending/basic.frag");

        //----------------- BLENDING -------------------//
        GLCall(glEnable(GL_BLEND));
        // Blending equation: source_color * source_factor + destination_color * destination_factor
        // usually source factor is the alpha channel of the source_color
        // and destination factor 1 - the alpha channel of the source_color
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    }

    TestBlending::~TestBlending()
    {
    }

    void TestBlending::OnUpdate(Enxus::Camera &camera)
    {
        m_Shader->Bind();
        // camera->SetViewProjMatrix(*m_Shader);
        m_Shader->SetMat4("uProj", camera.GetProjectionMatrix());
        m_Shader->SetMat4("uView", camera.GetViewMatrix());

        std::map<float, glm::vec3> m_SortedPositions;
        glm::vec3 cameraPos = camera.GetPos();
        for (auto pos : m_TransparentObjsPositions)
        {
            // negated value to loop the map in ascending object
            // It's a little lazy to write the map iterators and stuffs..
            float distance = -glm::length(camera.GetPos() - pos);
            m_SortedPositions[distance] = pos;
        }

        Enxus::Renderer::ClearColor(0.13f, 0.14f, 0.16f, 1.0f);

        m_Shader->Bind();
        //----------------- FLOOR -------------------//
        glm::mat4 model = glm::mat4(1.0f);
        m_Shader->SetMat4("uModel", model);
        Enxus::Renderer::DrawMesh(m_Floor, m_Shader);

        //----------------- BOXES -------------------//

        model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, -1.0f));
        m_Shader->SetMat4("uModel", model);
        Enxus::Renderer::DrawMesh(m_Box, m_Shader);

        model = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
        m_Shader->Bind();
        m_Shader->SetMat4("uModel", model);
        Enxus::Renderer::DrawMesh(m_Box, m_Shader);

        //----------------- GRASS -------------------//
        for (auto &[dist, pos] : m_SortedPositions)
        {
            model = glm::translate(glm::mat4(1.0f), pos);
            m_Shader->SetMat4("uModel", model);
            Enxus::Renderer::DrawMesh(m_TransparentObj, m_Shader);
        }
    };

    void TestBlending::OnImGuiRender()
    {
    }

}
