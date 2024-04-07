#include "Texture2D.h"
#include "utils.h"
#include "stb_image.h"
#include <iostream>

Texture2D::Texture2D(const std::string &texturePath)
    : m_RendererId(0), m_TexturePath(texturePath)
{

    GLCall(glGenTextures(1, &m_RendererId));
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));

    // Textures filters when scale up/down
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    // Textures wrapping (mirrored repeat)
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    LoadImage();
}
Texture2D::~Texture2D()
{
    GLCall(glDeleteTextures(1, &m_RendererId));
}

void Texture2D::LoadImage()
{
    // OpenGL expect the Y coordinate to start at the bottom left
    // instead of the top left, so this flip the Y coordinates of the image
    stbi_set_flip_vertically_on_load(true);

    m_LocalBuffer = stbi_load(m_TexturePath.c_str(), &m_Width, &m_Height, &m_NrChannels, 0);
    if (!m_LocalBuffer)
    {
        std::cout << "ERROR: Failed to load texture: " << m_TexturePath << std::endl;
        return;
    }
    // specify textures attributes
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, m_NrChannels > 3 ? GL_RGBA : GL_RGB, m_Width, m_Height, 0, m_NrChannels > 3 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer));

    // generate mipmaps using OpenGL built-in function
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    // free the second image data
    stbi_image_free(m_LocalBuffer);
}

void Texture2D::Bind(unsigned int slot) const
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererId));
}
void Texture2D::UnBind() const
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}