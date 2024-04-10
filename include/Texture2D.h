#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include <string>

enum TEXTURE_TYPE
{
    DIFFUSE,
    SPECULAR,
    NORMAL,
    // EMISSION,
    // Albedo, Metallic, Roughness etc
};

class Texture2D
{
private:
    unsigned int m_RendererId;
    int m_Width, m_Height, m_NrChannels;
    unsigned char *m_LocalBuffer; // texture data

    // debugging purposes
    std::string m_TexturePath;

    // texture map
    TEXTURE_TYPE m_Type;

public:
    Texture2D(const std::string &texturePath, TEXTURE_TYPE type = TEXTURE_TYPE::DIFFUSE);
    ~Texture2D();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    void SetType(TEXTURE_TYPE type) { m_Type = type; }
    unsigned int GetType() const { return m_Type; }

private:
    void LoadImage();
};

#endif