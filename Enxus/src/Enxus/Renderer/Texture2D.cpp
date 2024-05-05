#include "pch.h"
#include "Texture2D.h"
#include "utils.h"

namespace Enxus
{
    Texture2D::Texture2D(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height)
    {
        CreateTexture();
    }

    Texture2D::~Texture2D()
    {
        GLCall(glDeleteTextures(1, &m_RendererId));
    }

    void Texture2D::CreateTexture()
    {
        GLCall(glGenTextures(1, &m_RendererId));

        GLCall(glActiveTexture(GL_TEXTURE0));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));

        // Textures filters when scale up/down
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        // Textures wrapping (mirrored repeat)
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    }

    void Texture2D::SetData(void *data, uint32_t width, uint32_t height)
    {
        Bind();
        GLCall(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data));
    }

    void Texture2D::Bind(uint32_t slot) const
    {
        GLCall(glActiveTexture(GL_TEXTURE0 + slot));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
    }
    void Texture2D::Unbind() const
    {
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    }
}