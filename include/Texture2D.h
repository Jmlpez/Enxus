#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include <string>

enum Texture_Type
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
    Texture_Type m_Type;

public:
    Texture2D(const std::string &texturePath, Texture_Type type = Texture_Type::DIFFUSE);
    ~Texture2D();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    void SetType(Texture_Type type) { m_Type = type; }
    unsigned int GetType() const { return m_Type; }
    const std::string &GetPath() const { return m_TexturePath; }

private:
    void LoadImage();
};

#endif