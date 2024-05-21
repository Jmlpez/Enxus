#include "pch.h"
#include "SkyBox.h"
#include "utils.h"
#include "stb_image/stb_image.h"

namespace Enxus
{
    SkyBox::SkyBox()
        : m_RendererID(0)
    {
        static float skyboxVertices[] = {
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
        m_SkyBoxVAO = CreateRef<VertexArray>();

        // position (x, y, z)
        BufferLayout layout = {
            {ShaderDataType::Float3, "aPos"},
        };

        Ref<VertexBuffer> vertexBuffer = CreateRef<VertexBuffer>(skyboxVertices, sizeof(skyboxVertices));
        vertexBuffer->SetLayout(layout);
        m_SkyBoxVAO->AddVertexBuffer(vertexBuffer);
    }

    SkyBox::~SkyBox()
    {
        GLCall(glDeleteTextures(1, &m_RendererID));
    }

    void SkyBox::Bind() const
    {
        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
    }
    void SkyBox::Unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
    }

    void SkyBox::SetCubeMapFaces(const std::array<std::string, 6> &faces)
    {
        m_CubeMapFaces = faces;
        LoadCubeMap();
    }

    void SkyBox::LoadCubeMap()
    {
        if (m_RendererID)
        {
            GLCall(glDeleteTextures(1, &m_RendererID));
        }

        GLCall(glGenTextures(1, &m_RendererID));
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));

        // to avoid load the skybox upside down
        stbi_set_flip_vertically_on_load(false);

        int width, height, nrChannels;
        for (size_t i = 0; i < m_CubeMapFaces.size(); i++)
        {
            std::string fullPath = m_CubeMapFaces[i];
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
    }

}
