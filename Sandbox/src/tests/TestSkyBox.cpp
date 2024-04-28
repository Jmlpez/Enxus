#include "pch.h"
#include "TestSkyBox.h"
#include "stb_image/stb_image.h"

namespace OpenGLTest
{
    TestSkyBox::TestSkyBox() : m_SkyBoxRendererId(0)
    {

        //----------------- VAO/VBO -------------------//
        float skyboxVertices[] = {
            // positions
            -1.0f, 1.0f, -1.0f,  /* Face */
            -1.0f, -1.0f, -1.0f, /* Face */
            1.0f, -1.0f, -1.0f,  /* Face */
            1.0f, -1.0f, -1.0f,  /* Face */
            1.0f, 1.0f, -1.0f,   /* Face */
            -1.0f, 1.0f, -1.0f,  /* Face */

            -1.0f, -1.0f, 1.0f,  /* Face */
            -1.0f, -1.0f, -1.0f, /* Face */
            -1.0f, 1.0f, -1.0f,  /* Face */
            -1.0f, 1.0f, -1.0f,  /* Face */
            -1.0f, 1.0f, 1.0f,   /* Face */
            -1.0f, -1.0f, 1.0f,  /* Face */

            1.0f, -1.0f, -1.0f, /* Face */
            1.0f, -1.0f, 1.0f,  /* Face */
            1.0f, 1.0f, 1.0f,   /* Face */
            1.0f, 1.0f, 1.0f,   /* Face */
            1.0f, 1.0f, -1.0f,  /* Face */
            1.0f, -1.0f, -1.0f, /* Face */

            -1.0f, -1.0f, 1.0f, /* Face */
            -1.0f, 1.0f, 1.0f,  /* Face */
            1.0f, 1.0f, 1.0f,   /* Face */
            1.0f, 1.0f, 1.0f,   /* Face */
            1.0f, -1.0f, 1.0f,  /* Face */
            -1.0f, -1.0f, 1.0f, /* Face */

            -1.0f, 1.0f, -1.0f, /* Face */
            1.0f, 1.0f, -1.0f,  /* Face */
            1.0f, 1.0f, 1.0f,   /* Face */
            1.0f, 1.0f, 1.0f,   /* Face */
            -1.0f, 1.0f, 1.0f,  /* Face */
            -1.0f, 1.0f, -1.0f, /* Face */

            -1.0f, -1.0f, -1.0f, /* Face */
            -1.0f, -1.0f, 1.0f,  /* Face */
            1.0f, -1.0f, -1.0f,  /* Face */
            1.0f, -1.0f, -1.0f,  /* Face */
            -1.0f, -1.0f, 1.0f,  /* Face */
            1.0f, -1.0f, 1.0f    /* Face */
        };
        m_SkyBoxVAO = Enxus::CreateRef<Enxus::VertexArray>();
        m_SkyBoxVBO = Enxus::CreateRef<Enxus::VertexBuffer>(skyboxVertices, sizeof(skyboxVertices));

        Enxus::VertexBufferLayout layout;
        layout.Push(3, GL_FLOAT); // position (x, y, z)
        m_SkyBoxVAO->AddBuffer(*m_SkyBoxVBO, layout);

        //----------------- LOAD SKYBOX -------------------//

        std::array<std::string, 6> faces{
            "right.jpg",
            "left.jpg",
            "top.jpg",
            "bottom.jpg",
            "front.jpg",
            "back.jpg"};
        m_SkyBoxRendererId = LoadCubeMap(faces);

        //----------------- SHADER -------------------//

        m_SkyBoxShader = Enxus::CreateRef<Enxus::Shader>(
            "Sandbox/res/shaders/skybox/skybox.vert",
            "Sandbox/res/shaders/skybox/skybox.frag");

        // m_BoxShader = Enxus::CreateRef<Enxus::Shader>(
        //     "Sandbox/res/shaders/model/box.vert",
        //     "Sandbox/res/shaders/model/box.frag");
        m_BoxShader = Enxus::CreateRef<Enxus::Shader>(
            "Sandbox/res/shaders/skybox/reflect.vert",
            "Sandbox/res/shaders/skybox/reflect.frag");

        //----------------- BOX MODEL -------------------//
        m_Box = Enxus::CreateRef<Enxus::Model>("Sandbox/res/models/backpack/backpack.obj");
        m_BoxShader->Bind();
        m_BoxShader->SetMat4("uModel", glm::mat4(1.0f));
    }

    TestSkyBox::~TestSkyBox()
    {
    }

    uint32_t TestSkyBox::LoadCubeMap(std::array<std::string, 6> faces)
    {
        uint32_t textureId;
        GLCall(glGenTextures(1, &textureId));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureId));

        // to avoid load the skybox upside down
        stbi_set_flip_vertically_on_load(false);

        int width,
            height, nrChannels;
        for (size_t i = 0; i < faces.size(); i++)
        {
            std::string fullPath = "Sandbox/res/images/skybox/" + faces[i];
            unsigned char *imageData = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
            if (imageData)
            {
                GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData));
                stbi_image_free(imageData);
            }
            else
            {
                std::cout << "[SkyBox Error] Failed to load image : " << fullPath << std::endl;
                stbi_image_free(imageData);
                ASSERT(false);
            }
        }
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        return textureId;
    }

    void TestSkyBox::OnUpdate(Enxus::Camera &camera)
    {
        // draw the objects...
        {
            m_BoxShader->Bind();
            m_BoxShader->SetMat4("uView", camera.GetViewMatrix());
            m_BoxShader->SetMat4("uProj", camera.GetProjectionMatrix());

            m_BoxShader->SetVec3("uCameraPos", camera.GetPos());
            GLCall(glActiveTexture(GL_TEXTURE0));
            m_BoxShader->SetInt("uSkyBoxTexture", 0);

            Enxus::Renderer::DrawModel(m_Box, m_BoxShader);
        }
        // draw the skybox last
        {
            GLCall(glDepthMask(GL_FALSE));
            GLCall(glDepthFunc(GL_LEQUAL));

            glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera.GetViewMatrix()));

            m_SkyBoxShader->Bind();
            m_SkyBoxShader->SetMat4("uView", viewMatrix);
            m_SkyBoxShader->SetMat4("uProj", camera.GetProjectionMatrix());

            GLCall(glActiveTexture(GL_TEXTURE0));
            m_SkyBoxShader->SetInt("uSkyBoxTexture", 0);

            m_SkyBoxVAO->Bind();
            GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_SkyBoxRendererId));
            GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

            GLCall(glDepthMask(GL_TRUE)); // Restoring state
            GLCall(glDepthFunc(GL_LESS)); // Restoring state
        }
    }

    void TestSkyBox::OnImGuiRender()
    {
    }

}